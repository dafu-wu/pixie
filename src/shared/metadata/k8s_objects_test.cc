/*
 * Copyright 2018- The Pixie Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/shared/metadata/k8s_objects.h"

namespace px {
namespace md {

TEST(PodInfo, basic_accessors) {
  PodInfo pod_info("123", "pl", "pod1", PodQOSClass::kGuaranteed, PodPhase::kSucceeded,
                   {{PodConditionType::kReady, ConditionStatus::kTrue}}, "pod phase message",
                   "pod phase reason", "testnode", "testhost", "1.2.3.4");
  pod_info.set_start_time_ns(123);
  pod_info.set_stop_time_ns(256);

  EXPECT_EQ("123", pod_info.uid());
  EXPECT_EQ("pl", pod_info.ns());
  EXPECT_EQ("pod1", pod_info.name());
  EXPECT_EQ(PodQOSClass::kGuaranteed, pod_info.qos_class());
  EXPECT_EQ(PodPhase::kSucceeded, pod_info.phase());
  EXPECT_EQ("pod phase message", pod_info.phase_message());
  EXPECT_EQ("pod phase reason", pod_info.phase_reason());

  EXPECT_EQ("testnode", pod_info.node_name());
  EXPECT_EQ("testhost", pod_info.hostname());

  EXPECT_EQ(123, pod_info.start_time_ns());
  EXPECT_EQ(256, pod_info.stop_time_ns());

  EXPECT_EQ(K8sObjectType::kPod, pod_info.type());
}

TEST(PodInfo, debug_string) {
  PodInfo pod_info("123", "pl", "pod1", PodQOSClass::kGuaranteed, PodPhase::kRunning,
                   {{PodConditionType::kReady, ConditionStatus::kTrue}}, "pod phase message",
                   "pod phase reason", "testnode", "testhost", "1.1.1.1");
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(absl::Substitute("$0<Pod:ns=pl:name=pod1:uid=123:state=R>", Indent(i)),
              pod_info.DebugString(i));
  }

  pod_info.set_stop_time_ns(1000);
  EXPECT_EQ("<Pod:ns=pl:name=pod1:uid=123:state=S>", pod_info.DebugString());
}

TEST(PodInfo, add_delete_containers) {
  PodInfo pod_info("123", "pl", "pod1", PodQOSClass::kGuaranteed, PodPhase::kRunning,
                   {{PodConditionType::kReady, ConditionStatus::kTrue}}, "pod phase message",
                   "pod phase reason", "testnode", "testhost", "1.2.3.4");
  pod_info.AddContainer("ABCD");
  pod_info.AddContainer("ABCD2");
  pod_info.AddContainer("ABCD3");
  pod_info.RmContainer("ABCD");

  EXPECT_THAT(pod_info.containers(), testing::UnorderedElementsAre("ABCD2", "ABCD3"));

  pod_info.RmContainer("ABCD3");
  EXPECT_THAT(pod_info.containers(), testing::UnorderedElementsAre("ABCD2"));
}

TEST(PodInfo, clone) {
  PodInfo pod_info("123", "pl", "pod1", PodQOSClass::kBurstable, PodPhase::kRunning,
                   {{PodConditionType::kReady, ConditionStatus::kTrue}}, "pod phase message",
                   "pod phase reason", "testnode", "testhost", "1.2.3.4");
  pod_info.set_start_time_ns(123);
  pod_info.set_stop_time_ns(256);
  pod_info.AddContainer("ABCD");
  pod_info.AddContainer("ABCD2");

  EXPECT_EQ(PodQOSClass::kBurstable, pod_info.qos_class());

  std::unique_ptr<PodInfo> cloned(static_cast<PodInfo*>(pod_info.Clone().release()));
  EXPECT_EQ(cloned->uid(), pod_info.uid());
  EXPECT_EQ(cloned->name(), pod_info.name());
  EXPECT_EQ(cloned->ns(), pod_info.ns());
  EXPECT_EQ(cloned->qos_class(), pod_info.qos_class());

  EXPECT_EQ(cloned->start_time_ns(), pod_info.start_time_ns());
  EXPECT_EQ(cloned->stop_time_ns(), pod_info.stop_time_ns());

  EXPECT_EQ(cloned->type(), pod_info.type());
  EXPECT_EQ(cloned->containers(), pod_info.containers());
  EXPECT_EQ(cloned->phase(), pod_info.phase());
  EXPECT_EQ(cloned->phase_message(), pod_info.phase_message());
  EXPECT_EQ(cloned->phase_reason(), pod_info.phase_reason());
}

TEST(ContainerInfo, pod_id) {
  ContainerInfo cinfo("container1", "containername", ContainerState::kRunning,
                      ContainerType::kDocker, "container state message", "container state reason",
                      128 /*start_time*/);

  EXPECT_EQ("", cinfo.pod_id());
  cinfo.set_pod_id("pod1");
  EXPECT_EQ("pod1", cinfo.pod_id());
}

TEST(ContainerInfo, debug_string) {
  ContainerInfo cinfo("container1", "containername", ContainerState::kRunning,
                      ContainerType::kDocker, "container state message", "container state reason",
                      128);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(absl::Substitute("$0<Container:cid=container1:name=containername:pod_id=:state=R>",
                               Indent(i)),
              cinfo.DebugString(i));
  }

  cinfo.set_stop_time_ns(1000);
  EXPECT_EQ("<Container:cid=container1:name=containername:pod_id=:state=S>", cinfo.DebugString());
}

TEST(ContainerInfo, pids_are_time_sorted) {
  ContainerInfo orig("container1", "containername", ContainerState::kRunning,
                     ContainerType::kDocker, "container state message", "container state reason",
                     128 /*start_time*/);
  orig.set_pod_id("pod1");

  auto* upids = orig.mutable_active_upids();
  upids->emplace(UPID(1, 0, 200));
  upids->emplace(UPID(1, 1, 100));
  upids->emplace(UPID(1, 15, 400));
  upids->emplace(UPID(1, 15, 300));
  upids->emplace(UPID(1, 10, 300));
  upids->emplace(UPID(0, 1, 500));
  upids->emplace(UPID(0, 2, 300));
  upids->emplace(UPID(0, 3, 100));

  // UPIDs should be ordered by time.
  EXPECT_THAT(
      orig.active_upids(),
      testing::ElementsAre(UPID(0, 3, 100), UPID(1, 1, 100), UPID(1, 0, 200), UPID(0, 2, 300),
                           UPID(1, 10, 300), UPID(1, 15, 300), UPID(1, 15, 400), UPID(0, 1, 500)));
}

TEST(ContainerInfo, clone) {
  ContainerInfo orig("container1", "containername", ContainerState::kRunning,
                     ContainerType::kDocker, "container state message", "container state reason",
                     128 /*start_time*/);
  orig.set_pod_id("pod1");

  auto* upids = orig.mutable_active_upids();
  upids->emplace(UPID(1, 0, 200));
  upids->emplace(UPID(1, 1, 100));
  upids->emplace(UPID(1, 15, 300));

  auto cloned = orig.Clone();

  EXPECT_EQ(cloned->pod_id(), orig.pod_id());
  EXPECT_EQ(cloned->cid(), orig.cid());
  EXPECT_EQ(cloned->active_upids(), orig.active_upids());
  EXPECT_EQ(cloned->start_time_ns(), orig.start_time_ns());
  EXPECT_EQ(cloned->stop_time_ns(), orig.stop_time_ns());
  EXPECT_EQ(cloned->state(), orig.state());
  EXPECT_EQ(cloned->type(), orig.type());
  EXPECT_EQ(cloned->state_message(), orig.state_message());
  EXPECT_EQ(cloned->state_reason(), orig.state_reason());
}

TEST(ServiceInfo, basic_accessors) {
  ServiceInfo service_info("123", "pl", "service1");
  service_info.set_start_time_ns(123);
  service_info.set_stop_time_ns(256);
  service_info.set_external_ips(std::vector<std::string>{"127.0.0.1"});
  service_info.set_cluster_ip("127.0.0.2");

  EXPECT_EQ("123", service_info.uid());
  EXPECT_EQ("pl", service_info.ns());
  EXPECT_EQ("service1", service_info.name());

  EXPECT_EQ(123, service_info.start_time_ns());
  EXPECT_EQ(256, service_info.stop_time_ns());

  EXPECT_EQ(K8sObjectType::kService, service_info.type());

  EXPECT_EQ("127.0.0.2", service_info.cluster_ip());
  EXPECT_EQ(std::vector<std::string>{"127.0.0.1"}, service_info.external_ips());
}

TEST(ServiceInfo, debug_string) {
  ServiceInfo service_info("123", "pl", "service1");
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(absl::Substitute("$0<Service:ns=pl:name=service1:uid=123:state=R>", Indent(i)),
              service_info.DebugString(i));
  }

  service_info.set_stop_time_ns(1000);
  EXPECT_EQ("<Service:ns=pl:name=service1:uid=123:state=S>", service_info.DebugString());
}

TEST(ServiceInfo, clone) {
  ServiceInfo service_info("123", "pl", "service1");
  service_info.set_start_time_ns(123);
  service_info.set_stop_time_ns(256);
  service_info.set_external_ips(std::vector<std::string>{"127.0.0.1"});
  service_info.set_cluster_ip("127.0.0.2");

  std::unique_ptr<ServiceInfo> cloned(static_cast<ServiceInfo*>(service_info.Clone().release()));
  EXPECT_EQ(cloned->uid(), service_info.uid());
  EXPECT_EQ(cloned->name(), service_info.name());
  EXPECT_EQ(cloned->ns(), service_info.ns());

  EXPECT_EQ(cloned->start_time_ns(), service_info.start_time_ns());
  EXPECT_EQ(cloned->stop_time_ns(), service_info.stop_time_ns());

  EXPECT_EQ(cloned->type(), service_info.type());
  EXPECT_EQ("127.0.0.2", cloned->cluster_ip());
  EXPECT_EQ(std::vector<std::string>{"127.0.0.1"}, cloned->external_ips());
}

}  // namespace md
}  // namespace px
