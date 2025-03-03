/*
 * Copyright 2022- The Pixie Authors.
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

#pragma once

#include <string>
#include "src/stirling/source_connectors/socket_tracer/protocols/amqp/types_gen.h"

#include "src/common/base/base.h"
#include "src/common/json/json.h"
#include "src/stirling/utils/binary_decoder.h"

namespace px {
namespace stirling {
namespace protocols {
namespace amqp {

struct AMQPConnectionStart {
  uint8_t version_major = 0;
  uint8_t version_minor = 0;
  std::string server_properties = "";
  std::string mechanisms = "";
  std::string locales = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("version_major", version_major);
    builder->WriteKV("version_minor", version_minor);
    // TODO(vsrivatsa): support KV for server_properties field table type
    builder->WriteKV("mechanisms", mechanisms);
    builder->WriteKV("locales", locales);
  }
};

struct AMQPConnectionStartOk {
  std::string client_properties = "";
  std::string mechanism = "";
  std::string response = "";
  std::string locale = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    // TODO(vsrivatsa): support KV for client_properties field table type
    builder->WriteKV("mechanism", mechanism);
    builder->WriteKV("response", response);
    builder->WriteKV("locale", locale);
  }
};

struct AMQPConnectionSecure {
  std::string challenge = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("challenge", challenge); }
};

struct AMQPConnectionSecureOk {
  std::string response = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("response", response); }
};

struct AMQPConnectionTune {
  uint16_t channel_max = 0;
  uint32_t frame_max = 0;
  uint16_t heartbeat = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("channel_max", channel_max);
    builder->WriteKV("frame_max", frame_max);
    builder->WriteKV("heartbeat", heartbeat);
  }
};

struct AMQPConnectionTuneOk {
  uint16_t channel_max = 0;
  uint32_t frame_max = 0;
  uint16_t heartbeat = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("channel_max", channel_max);
    builder->WriteKV("frame_max", frame_max);
    builder->WriteKV("heartbeat", heartbeat);
  }
};

struct AMQPConnectionOpen {
  std::string virtual_host = "";
  std::string reserved_1 = "";
  bool reserved_2 = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("virtual_host", virtual_host);
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("reserved_2", reserved_2);
  }
};

struct AMQPConnectionOpenOk {
  std::string reserved_1 = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
  }
};

struct AMQPConnectionClose {
  uint16_t reply_code = 0;
  std::string reply_text = "";
  uint16_t class_id = 0;
  uint16_t method_id = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reply_code", reply_code);
    builder->WriteKV("reply_text", reply_text);
    builder->WriteKV("class_id", class_id);
    builder->WriteKV("method_id", method_id);
  }
};

struct AMQPConnectionCloseOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPConnectionContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
  }
};

struct AMQPChannelOpen {
  std::string reserved_1 = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
  }
};

struct AMQPChannelOpenOk {
  std::string reserved_1 = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
  }
};

struct AMQPChannelFlow {
  bool active = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("active", active); }
};

struct AMQPChannelFlowOk {
  bool active = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("active", active); }
};

struct AMQPChannelClose {
  uint16_t reply_code = 0;
  std::string reply_text = "";
  uint16_t class_id = 0;
  uint16_t method_id = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reply_code", reply_code);
    builder->WriteKV("reply_text", reply_text);
    builder->WriteKV("class_id", class_id);
    builder->WriteKV("method_id", method_id);
  }
};

struct AMQPChannelCloseOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPChannelContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
  }
};

struct AMQPExchangeDeclare {
  uint16_t reserved_1 = 0;
  std::string exchange = "";
  std::string type = "";
  bool passive = 0;
  bool durable = 0;
  bool reserved_2 = 0;
  bool reserved_3 = 0;
  bool no_wait = 0;
  std::string arguments = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("type", type);
    builder->WriteKV("passive", passive);
    builder->WriteKV("durable", durable);
    builder->WriteKV("reserved_2", reserved_2);
    builder->WriteKV("reserved_3", reserved_3);
    builder->WriteKV("no_wait", no_wait);
    // TODO(vsrivatsa): support KV for arguments field table type
  }
};

struct AMQPExchangeDeclareOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPExchangeDelete {
  uint16_t reserved_1 = 0;
  std::string exchange = "";
  bool if_unused = 0;
  bool no_wait = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("if_unused", if_unused);
    builder->WriteKV("no_wait", no_wait);
  }
};

struct AMQPExchangeDeleteOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPExchangeContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
  }
};

struct AMQPQueueDeclare {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  bool passive = 0;
  bool durable = 0;
  bool exclusive = 0;
  bool auto_delete = 0;
  bool no_wait = 0;
  std::string arguments = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("passive", passive);
    builder->WriteKV("durable", durable);
    builder->WriteKV("exclusive", exclusive);
    builder->WriteKV("auto_delete", auto_delete);
    builder->WriteKV("no_wait", no_wait);
    // TODO(vsrivatsa): support KV for arguments field table type
  }
};

struct AMQPQueueDeclareOk {
  std::string queue = "";
  uint32_t message_count = 0;
  uint32_t consumer_count = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("queue", queue);
    builder->WriteKV("message_count", message_count);
    builder->WriteKV("consumer_count", consumer_count);
  }
};

struct AMQPQueueBind {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  std::string exchange = "";
  std::string routing_key = "";
  bool no_wait = 0;
  std::string arguments = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
    builder->WriteKV("no_wait", no_wait);
    // TODO(vsrivatsa): support KV for arguments field table type
  }
};

struct AMQPQueueBindOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPQueueUnbind {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  std::string exchange = "";
  std::string routing_key = "";
  std::string arguments = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
    // TODO(vsrivatsa): support KV for arguments field table type
  }
};

struct AMQPQueueUnbindOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPQueuePurge {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  bool no_wait = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("no_wait", no_wait);
  }
};

struct AMQPQueuePurgeOk {
  uint32_t message_count = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("message_count", message_count);
  }
};

struct AMQPQueueDelete {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  bool if_unused = 0;
  bool if_empty = 0;
  bool no_wait = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("if_unused", if_unused);
    builder->WriteKV("if_empty", if_empty);
    builder->WriteKV("no_wait", no_wait);
  }
};

struct AMQPQueueDeleteOk {
  uint32_t message_count = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("message_count", message_count);
  }
};

struct AMQPQueueContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
  }
};

struct AMQPBasicQos {
  uint32_t prefetch_size = 0;
  uint16_t prefetch_count = 0;
  bool global = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("prefetch_size", prefetch_size);
    builder->WriteKV("prefetch_count", prefetch_count);
    builder->WriteKV("global", global);
  }
};

struct AMQPBasicQosOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPBasicConsume {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  std::string consumer_tag = "";
  bool no_local = 0;
  bool no_ack = 0;
  bool exclusive = 0;
  bool no_wait = 0;
  std::string arguments = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("consumer_tag", consumer_tag);
    builder->WriteKV("no_local", no_local);
    builder->WriteKV("no_ack", no_ack);
    builder->WriteKV("exclusive", exclusive);
    builder->WriteKV("no_wait", no_wait);
    // TODO(vsrivatsa): support KV for arguments field table type
  }
};

struct AMQPBasicConsumeOk {
  std::string consumer_tag = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("consumer_tag", consumer_tag);
  }
};

struct AMQPBasicCancel {
  std::string consumer_tag = "";
  bool no_wait = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("consumer_tag", consumer_tag);
    builder->WriteKV("no_wait", no_wait);
  }
};

struct AMQPBasicCancelOk {
  std::string consumer_tag = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("consumer_tag", consumer_tag);
  }
};

struct AMQPBasicPublish {
  uint16_t reserved_1 = 0;
  std::string exchange = "";
  std::string routing_key = "";
  bool mandatory = 0;
  bool immediate = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
    builder->WriteKV("mandatory", mandatory);
    builder->WriteKV("immediate", immediate);
  }
};

struct AMQPBasicReturn {
  uint16_t reply_code = 0;
  std::string reply_text = "";
  std::string exchange = "";
  std::string routing_key = "";
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reply_code", reply_code);
    builder->WriteKV("reply_text", reply_text);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
  }
};

struct AMQPBasicDeliver {
  std::string consumer_tag = "";
  uint64_t delivery_tag = 0;
  bool redelivered = 0;
  std::string exchange = "";
  std::string routing_key = "";
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("consumer_tag", consumer_tag);
    builder->WriteKV("delivery_tag", delivery_tag);
    builder->WriteKV("redelivered", redelivered);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
  }
};

struct AMQPBasicGet {
  uint16_t reserved_1 = 0;
  std::string queue = "";
  bool no_ack = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
    builder->WriteKV("queue", queue);
    builder->WriteKV("no_ack", no_ack);
  }
};

struct AMQPBasicGetOk {
  uint64_t delivery_tag = 0;
  bool redelivered = 0;
  std::string exchange = "";
  std::string routing_key = "";
  uint32_t message_count = 0;
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("delivery_tag", delivery_tag);
    builder->WriteKV("redelivered", redelivered);
    builder->WriteKV("exchange", exchange);
    builder->WriteKV("routing_key", routing_key);
    builder->WriteKV("message_count", message_count);
  }
};

struct AMQPBasicGetEmpty {
  std::string reserved_1 = "";
  bool synchronous = 1;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("reserved_1", reserved_1);
  }
};

struct AMQPBasicAck {
  uint64_t delivery_tag = 0;
  bool multiple = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("delivery_tag", delivery_tag);
    builder->WriteKV("multiple", multiple);
  }
};

struct AMQPBasicReject {
  uint64_t delivery_tag = 0;
  bool requeue = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("delivery_tag", delivery_tag);
    builder->WriteKV("requeue", requeue);
  }
};

struct AMQPBasicRecoverAsync {
  bool requeue = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("requeue", requeue); }
};

struct AMQPBasicRecover {
  bool requeue = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const { builder->WriteKV("requeue", requeue); }
};

struct AMQPBasicRecoverOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPBasicContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  std::string content_type = "";
  std::string content_encoding = "";
  std::string headers = "";
  uint8_t delivery_mode = 0;
  uint8_t priority = 0;
  std::string correlation_id = "";
  std::string reply_to = "";
  std::string expiration = "";
  std::string message_id = "";
  time_t timestamp = 0;
  std::string type = "";
  std::string user_id = "";
  std::string app_id = "";
  std::string reserved = "";
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
    builder->WriteKV("content_type", content_type);
    builder->WriteKV("content_encoding", content_encoding);
    // TODO(vsrivatsa): support KV for headers field table type
    builder->WriteKV("delivery_mode", delivery_mode);
    builder->WriteKV("priority", priority);
    builder->WriteKV("correlation_id", correlation_id);
    builder->WriteKV("reply_to", reply_to);
    builder->WriteKV("expiration", expiration);
    builder->WriteKV("message_id", message_id);
    builder->WriteKV("timestamp", timestamp);
    builder->WriteKV("type", type);
    builder->WriteKV("user_id", user_id);
    builder->WriteKV("app_id", app_id);
    builder->WriteKV("reserved", reserved);
  }
};

struct AMQPTxSelect {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxSelectOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxCommit {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxCommitOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxRollback {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxRollbackOk {
  bool synchronous = 1;

  void ToJSON([[maybe_unused]] utils::JSONObjectBuilder* builder) const {}
};

struct AMQPTxContentHeader {
  uint64_t body_size = 0;
  uint16_t property_flags = 0;
  bool synchronous = 0;

  void ToJSON(utils::JSONObjectBuilder* builder) const {
    builder->WriteKV("body_size", body_size);
    builder->WriteKV("property_flags", property_flags);
  }
};

template <typename T>
std::string ToString(T obj) {
  utils::JSONObjectBuilder json_object_builder;
  obj.ToJSON(&json_object_builder);
  return json_object_builder.GetString();
}
Status ProcessReq(Frame* req);

}  // namespace amqp
}  // namespace protocols
}  // namespace stirling
}  // namespace px
