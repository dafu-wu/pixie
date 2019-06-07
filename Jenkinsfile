/**
 * Jenkins build definition. This file defines the entire build pipeline.
 */
import java.net.URLEncoder;
import groovy.json.JsonBuilder

/**
  * We expect the following parameters to be defined (for code review builds):
  *    PHID: Which should be the buildTargetPHID from Harbormaster.
  *    INITIATOR_PHID: Which is the PHID of the initiator (ie. Differential)
  *    API_TOKEN: The api token to use to communicate with Phabricator
  *    REVISION: The revision ID of the Differential.
  */

// NOTE: We use these without a def/type because that way Groovy will treat these as
// global variables.
PHAB_URL = 'https://phab.pixielabs.ai'
PHAB_API_URL = "${PHAB_URL}/api"

// Restrict build to source code, since otherwise bazel seems to build all our deps.
BAZEL_SRC_FILES_PATH = "//src/..."
// ASAN/TSAN only work for CC code.
// TODO(zasgar): This query selects only cc binaries. After GO ASAN/TSAN works, we can update the ASAN/TSAN builds
// to include all binaries.
// This line also contains a hack to filter out cgo object files, assuming the object files have the _cgo_.o suffix.
BAZEL_CC_QUERY = "`bazel query 'kind(\"cc_(binary|test) rule\", src/...)' | grep -v '_cgo_.o\$'`"
SRC_STASH_NAME = "${BUILD_TAG}_src"
DEV_DOCKER_IMAGE = 'pl-dev-infra/dev_image'

// Sometimes docker fetches fail, so we just do a retry. This can be optimized to just
// retry on docker failues, but not worth it now.
JENKINS_RETRIES = 2;

// This variable store the dev docker image that we need to parse before running any docker steps.
devDockerImageWithTag = ''

/**
  * @brief Generates URL for harbormaster.
  */
def harborMasterUrl = {
  method ->
    url = "${PHAB_API_URL}/${method}?api.token=${params.API_TOKEN}" +
            "&buildTargetPHID=${params.PHID}"
    return url
}

/**
 * @brief Sends build status to Phabricator.
 */
def sendBuildStatus = {
  build_status ->
    def url = harborMasterUrl("harbormaster.sendmessage")
    def body = "type=${build_status}"
    httpRequest consoleLogResponseBody: true,
      contentType: 'APPLICATION_FORM',
      httpMode: 'POST',
      requestBody: body,
      responseHandle: 'NONE',
      url: url,
      validResponseCodes: '200'
}

/**
  * @brief Add build info to harbormaster and badge to Jenkins.
  */
def addBuildInfo = {
  def encodedDisplayUrl = URLEncoder.encode(env.RUN_DISPLAY_URL, 'UTF-8')
  def url = harborMasterUrl("harbormaster.createartifact")
  def body = ""
  body += "&buildTargetPHID=${params.PHID}"
  body += '&artifactKey=jenkins.uri'
  body += '&artifactType=uri'
  body += "&artifactData[uri]=${encodedDisplayUrl}"
  body += '&artifactData[name]=Jenkins'
  body += '&artifactData[ui.external]=true'

  httpRequest consoleLogResponseBody: true,
    contentType: 'APPLICATION_FORM',
    httpMode: 'POST',
    requestBody: body,
    responseHandle: 'NONE',
    url: url,
    validResponseCodes: '200'

  def text = ""
  def link = ""
  // Either a revision of a commit to master.
  if (params.REVISION) {
    def revisionId = "D${REVISION}"
    text = revisionId
    link = "${PHAB_URL}/${revisionId}"
  } else {
    text = params.PHAB_COMMIT.substring(0, 7)
    link = "${PHAB_URL}/rPLM${env.PHAB_COMMIT}"
  }
  addShortText(text: text,
    background: "transparent",
    border: 0,
    borderColor: "transparent",
    color: "#1FBAD6",
    link: link)

}

def addStoryBookLink = {
  def encodedDisplayUrl = URLEncoder.encode(env.BUILD_URL + '/ui-storybook', 'UTF-8')
  def url = harborMasterUrl("harbormaster.createartifact")
  def body = ""
  body += "&buildTargetPHID=${params.PHID}"
  body += '&artifactKey=storybook.uri'
  body += '&artifactType=uri'
  body += "&artifactData[uri]=${encodedDisplayUrl}"
  body += '&artifactData[name]=StoryBook'
  body += '&artifactData[ui.external]=true'

  httpRequest consoleLogResponseBody: true,
    contentType: 'APPLICATION_FORM',
    httpMode: 'POST',
    requestBody: body,
    responseHandle: 'NONE',
    url: url,
    validResponseCodes: '200'
}

/**
 * @brief Returns true if it's a phabricator triggered build.
 *  This could either be code review build or master commit.
 */
def isPhabricatorTriggeredBuild() {
  return params.PHID != null && params.PHID != ""
}

def codeReviewPreBuild = {
  sendBuildStatus('work')
  addBuildInfo()
}

def codeReviewPostBuild = {
  if (currentBuild.result == "SUCCESS") {
    sendBuildStatus('pass')
  } else {
    sendBuildStatus('fail')
  }
  addStoryBookLink()
}

def writeBazelRCFile() {
  def bazelRcFile = [
    'common --color=yes',
    // Build arguments.
    'build --announce_rc',
    'build --verbose_failures',
    'build --jobs=16',
    // Build remote jobs setup.
    'build --google_default_credentials',
    // Use GCS as cache as this is more scalable than our machine.
    "build --remote_http_cache=https://storage.googleapis.com/bazel-cache-pl",
    'build --remote_local_fallback=true',
    'build --remote_local_fallback_strategy=local',
    'build --remote_timeout=10',
    'build --experimental_remote_retry',
    // Test remote jobs setup.
    'test --remote_timeout=10',
    'test --remote_local_fallback=true',
    'test --remote_local_fallback_strategy=local',
    'test --test_output=errors',
    // Other test args.
    'test --verbose_failures',
  ].join('\n')
  writeFile file: "jenkins.bazelrc", text: "${bazelRcFile}"
}

def builders = [:]


/**
  * Our default docker step :
  *   1. Deletes old directory.
  *   2. Checks out new code stash.
  *   3. Starts docker container.
  *   4. Runs the passed in body.
  */
def dockerStepWithCode(String dockerConfig = '', Closure body) {
  retry(JENKINS_RETRIES) {
    node {
      deleteDir()
      unstash SRC_STASH_NAME
      docker.withRegistry('https://gcr.io', 'gcr:pl-dev-infra') {
        docker.image(devDockerImageWithTag).inside(dockerConfig) {
          body()
        }
      }
    }
  }
}

builders['Build & Test (dbg)'] = {
  dockerStepWithCode {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "bazel test --compilation_mode=dbg ${BAZEL_SRC_FILES_PATH}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-dbg-testlogs', includes: "bazel-testlogs-archive/**"
  }
}


builders['Build & Test (opt)'] = {
  dockerStepWithCode {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "bazel test --compilation_mode=opt ${BAZEL_SRC_FILES_PATH}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-opt-testlogs', includes: "bazel-testlogs-archive/**"
  }
}


builders['Build & Test (gcc:opt)'] = {
  dockerStepWithCode {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "CC=gcc CXX=g++ bazel test --compilation_mode=opt ${BAZEL_SRC_FILES_PATH}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-gcc-opt-testlogs', includes: "bazel-testlogs-archive/**"
  }
}


builders['Build & Test (bpf)'] = {
  dockerStepWithCode(
    '--privileged --pid=host --volume /lib/modules:/lib/modules --volume /usr/src:/usr/src --volume /sys:/sys') {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "bazel test --compilation_mode=opt --strategy=TestRunner=standalone --config=bpf ${BAZEL_SRC_FILES_PATH}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-bpf-testlogs', includes: "bazel-testlogs-archive/**"
  }
}


builders['Build & Test (clang-tidy)'] = {
  dockerStepWithCode {
    sh 'scripts/bazel_fetch_retry.sh'
    sh 'scripts/run_clang_tidy.sh'
    stash name: 'build-clang-tidy-logs', includes: "clang_tidy.log"
  }
}


// Only run coverage on master test.
if (env.JOB_NAME == "pixielabs-master") {
  builders['Build & Test (gcc:coverage)'] = {
    dockerStepWithCode {
      sh 'scripts/bazel_fetch_retry.sh'
      sh 'scripts/collect_coverage.sh -u -t ${CODECOV_TOKEN} -b master -c `cat GIT_COMMIT`'
      sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
      stash name: 'build-gcc-coverage-testlogs', includes: "bazel-testlogs-archive/**"
    }
  }
}


/********************************************
 * For now restrict the ASAN and TSAN builds to carnot. There is a bug in go(or llvm) preventing linking:
 * https://github.com/golang/go/issues/27110
 * TODO(zasgar): Fix after above is resolved.
 ********************************************/
builders['Build & Test (asan)'] = {
  dockerStepWithCode('--cap-add=SYS_PTRACE') {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "bazel test --config=asan ${BAZEL_CC_QUERY}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-asan-testlogs', includes: "bazel-testlogs-archive/**"
  }
}

builders['Build & Test (tsan)'] = {
  dockerStepWithCode {
    sh 'scripts/bazel_fetch_retry.sh'
    sh "bazel test --config=tsan ${BAZEL_CC_QUERY}"
    sh 'cp -a bazel-testlogs/ bazel-testlogs-archive'
    stash name: 'build-tsan-testlogs', includes: "bazel-testlogs-archive/**"
  }
}

builders['Linting'] = {
  dockerStepWithCode {
    sh 'arc lint --everything'
  }
}

builders['Build & Test UI'] = {
  dockerStepWithCode {
    sh '''
      cd src/ui
      yarn install --prefer_offline
      jest

      # Build story book static files.
      yarn run storybook_static
    '''
    stash name: 'build-ui-testlogs', includes: "src/ui/junit.xml"
    stash name: 'build-ui-storybook-static', includes: "src/ui/storybook_static/**"
  }
}

/********************************************
 * The build script starts here.
 ********************************************/
if (isPhabricatorTriggeredBuild()) {
  codeReviewPreBuild()
}

node {
  currentBuild.result = 'SUCCESS'
  deleteDir()
  try {
    stage('Checkout code') {
      checkout scm
      sh '''
        printenv
        # Store the GIT commit in a file, since the git plugin has issues with
        # the Jenkins pipeline system.
        git rev-parse HEAD > GIT_COMMIT
      '''
      writeBazelRCFile()

      // Get docker image tag.
      def properties = readProperties file: 'docker.properties'
      devDockerImageWithTag = DEV_DOCKER_IMAGE + ":${properties.DOCKER_IMAGE_TAG}"

      // Excluding default excludes also stashes the .git folder which downstream steps need.
      stash name: SRC_STASH_NAME, useDefaultExcludes: false
    }
    stage('Build Steps') {
      parallel(builders)
    }
    stage('Archive') {
      dir ('build-opt-testlogs') {
        unstash 'build-opt-testlogs'
      }
      dir ('build-gcc-opt-testlogs') {
        unstash 'build-gcc-opt-testlogs'
      }
      dir ('build-bpf-testlogs') {
        unstash 'build-bpf-testlogs'
      }
      dir ('build-dbg-testlogs') {
        unstash 'build-dbg-testlogs'
      }
      dir ('build-asan-testlogs') {
        unstash 'build-asan-testlogs'
      }
      dir ('build-tsan-testlogs') {
        unstash 'build-tsan-testlogs'
      }
      dir ('build-ui-testlogs') {
        unstash 'build-ui-testlogs'
      }
      dir ('build-ui-storybook-static') {
        unstash 'build-ui-storybook-static'
      }
      dir ('build-clang-tidy-logs') {
        unstash 'build-clang-tidy-logs'
      }

      if (env.JOB_NAME == "pixielabs-master") {
        dir ('build-gcc-coverage-testlogs') {
          unstash 'build-gcc-coverage-testlogs'
        }
      }

      archiveArtifacts artifacts: 'build-clang-tidy-logs/**', fingerprint: true

      publishHTML([allowMissing: false,
        alwaysLinkToLastBuild: true,
        keepAll: true,
        reportDir: 'build-ui-storybook-static/src/ui/storybook_static',
        reportFiles: 'index.html',
        reportName: 'ui-storybook'
      ])

      step([
        $class: 'XUnitBuilder',
        thresholds: [
          [
            $class: 'FailedThreshold',
            unstableThreshold: '1'
          ]
        ],
        tools: [
          [
            $class: 'GoogleTestType',
            pattern: "build*/bazel-testlogs-archive/**/*.xml"
          ]
        ]
      ])

      step([
        $class: 'XUnitBuilder',
        thresholds: [
          [
            $class: 'FailedThreshold',
            unstableThreshold: '1'
          ]
        ],
        tools: [
          [
            $class: 'JUnitType',
            pattern: "build-ui-testlogs/src/ui/junit.xml"
          ]
        ]
      ])
    }
  }
  catch(err) {
    currentBuild.result = 'FAILURE'
    echo "Exception thrown:\n ${err}"
    echo "Stacktrace:"
    err.printStackTrace()
  }
  finally {
    if (isPhabricatorTriggeredBuild()) {
      codeReviewPostBuild()
    }
  }
}
