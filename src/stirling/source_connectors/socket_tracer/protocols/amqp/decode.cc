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

#include "src/stirling/source_connectors/socket_tracer/protocols/amqp/decode.h"

#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "src/common/base/base.h"
#include "src/stirling/source_connectors/socket_tracer/protocols/amqp/types_gen.h"
#include "src/stirling/utils/binary_decoder.h"

namespace px {
namespace stirling {
namespace protocols {
namespace amqp {

StatusOr<std::string> ExtractShortString(BinaryDecoder* decoder) {
  // Short string defined as 2*OCTET(short-uint)
  PL_ASSIGN_OR_RETURN(uint8_t len, decoder->ExtractInt<uint8_t>());
  return decoder->ExtractString(len);
}

StatusOr<std::string> ExtractLongString(BinaryDecoder* decoder) {
  // Long string defined as 4*OCTET(short-uint)
  PL_ASSIGN_OR_RETURN(uint32_t len, decoder->ExtractInt<uint32_t>());
  return decoder->ExtractString(len);
}
StatusOr<bool> ExtractNthBit(BinaryDecoder* decoder, int n) {
  // Extract Value at Nth bit
  return decoder->Buf()[0] >> n & 1;
}

Status ExtractAMQPConnectionStart(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionStart r;
  PL_ASSIGN_OR_RETURN(r.version_major, decoder->ExtractChar<uint8_t>());
  PL_ASSIGN_OR_RETURN(r.version_minor, decoder->ExtractChar<uint8_t>());
  PL_ASSIGN_OR_RETURN(r.server_properties, ExtractLongString(decoder));
  PL_ASSIGN_OR_RETURN(r.mechanisms, ExtractLongString(decoder));
  PL_ASSIGN_OR_RETURN(r.locales, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionStartOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionStartOk r;
  PL_ASSIGN_OR_RETURN(r.client_properties, ExtractLongString(decoder));
  PL_ASSIGN_OR_RETURN(r.mechanism, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.response, ExtractLongString(decoder));
  PL_ASSIGN_OR_RETURN(r.locale, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionSecure(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionSecure r;
  PL_ASSIGN_OR_RETURN(r.challenge, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionSecureOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionSecureOk r;
  PL_ASSIGN_OR_RETURN(r.response, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionTune(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionTune r;
  PL_ASSIGN_OR_RETURN(r.channel_max, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.frame_max, decoder->ExtractInt<uint32_t>());
  PL_ASSIGN_OR_RETURN(r.heartbeat, decoder->ExtractInt<uint16_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionTuneOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionTuneOk r;
  PL_ASSIGN_OR_RETURN(r.channel_max, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.frame_max, decoder->ExtractInt<uint32_t>());
  PL_ASSIGN_OR_RETURN(r.heartbeat, decoder->ExtractInt<uint16_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionOpen(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionOpen r;
  PL_ASSIGN_OR_RETURN(r.virtual_host, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.reserved_1, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.reserved_2, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionOpenOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionOpenOk r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionClose(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionClose r;
  PL_ASSIGN_OR_RETURN(r.reply_code, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.reply_text, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.class_id, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.method_id, decoder->ExtractInt<uint16_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionCloseOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionCloseOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPConnectionContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPConnectionContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPChannelOpen(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelOpen r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPChannelOpenOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelOpenOk r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPChannelFlow(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelFlow r;
  PL_ASSIGN_OR_RETURN(r.active, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPChannelFlowOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelFlowOk r;
  PL_ASSIGN_OR_RETURN(r.active, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPChannelClose(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelClose r;
  PL_ASSIGN_OR_RETURN(r.reply_code, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.reply_text, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.class_id, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.method_id, decoder->ExtractInt<uint16_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPChannelCloseOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelCloseOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPChannelContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPChannelContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPExchangeDeclare(BinaryDecoder* decoder, Frame* frame) {
  AMQPExchangeDeclare r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.type, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.passive, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.durable, ExtractNthBit(decoder, 1));
  PL_ASSIGN_OR_RETURN(r.reserved_2, ExtractNthBit(decoder, 2));
  PL_ASSIGN_OR_RETURN(r.reserved_3, ExtractNthBit(decoder, 3));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 4));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.arguments, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPExchangeDeclareOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPExchangeDeclareOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPExchangeDelete(BinaryDecoder* decoder, Frame* frame) {
  AMQPExchangeDelete r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.if_unused, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 1));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPExchangeDeleteOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPExchangeDeleteOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPExchangeContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPExchangeContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPQueueDeclare(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueDeclare r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.passive, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.durable, ExtractNthBit(decoder, 1));
  PL_ASSIGN_OR_RETURN(r.exclusive, ExtractNthBit(decoder, 2));
  PL_ASSIGN_OR_RETURN(r.auto_delete, ExtractNthBit(decoder, 3));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 4));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.arguments, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueDeclareOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueDeclareOk r;
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.message_count, decoder->ExtractInt<uint32_t>());
  PL_ASSIGN_OR_RETURN(r.consumer_count, decoder->ExtractInt<uint32_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueBind(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueBind r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.arguments, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueBindOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueBindOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueUnbind(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueUnbind r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.arguments, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueUnbindOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueUnbindOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueuePurge(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueuePurge r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueuePurgeOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueuePurgeOk r;
  PL_ASSIGN_OR_RETURN(r.message_count, decoder->ExtractInt<uint32_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueDelete(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueDelete r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.if_unused, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.if_empty, ExtractNthBit(decoder, 1));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 2));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueDeleteOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueDeleteOk r;
  PL_ASSIGN_OR_RETURN(r.message_count, decoder->ExtractInt<uint32_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPQueueContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPQueueContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicQos(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicQos r;
  PL_ASSIGN_OR_RETURN(r.prefetch_size, decoder->ExtractInt<uint32_t>());
  PL_ASSIGN_OR_RETURN(r.prefetch_count, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.global, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicQosOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicQosOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicConsume(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicConsume r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.consumer_tag, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.no_local, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.no_ack, ExtractNthBit(decoder, 1));
  PL_ASSIGN_OR_RETURN(r.exclusive, ExtractNthBit(decoder, 2));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 3));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.arguments, ExtractLongString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicConsumeOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicConsumeOk r;
  PL_ASSIGN_OR_RETURN(r.consumer_tag, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicCancel(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicCancel r;
  PL_ASSIGN_OR_RETURN(r.consumer_tag, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.no_wait, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicCancelOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicCancelOk r;
  PL_ASSIGN_OR_RETURN(r.consumer_tag, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicPublish(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicPublish r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.mandatory, ExtractNthBit(decoder, 0));
  PL_ASSIGN_OR_RETURN(r.immediate, ExtractNthBit(decoder, 1));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicReturn(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicReturn r;
  PL_ASSIGN_OR_RETURN(r.reply_code, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.reply_text, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicDeliver(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicDeliver r;
  PL_ASSIGN_OR_RETURN(r.consumer_tag, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.delivery_tag, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(r.redelivered, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicGet(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicGet r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(r.queue, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.no_ack, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicGetOk(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicGetOk r;
  PL_ASSIGN_OR_RETURN(r.delivery_tag, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(r.redelivered, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  PL_ASSIGN_OR_RETURN(r.exchange, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.routing_key, ExtractShortString(decoder));
  PL_ASSIGN_OR_RETURN(r.message_count, decoder->ExtractInt<uint32_t>());
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicGetEmpty(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicGetEmpty r;
  PL_ASSIGN_OR_RETURN(r.reserved_1, ExtractShortString(decoder));
  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicAck(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicAck r;
  PL_ASSIGN_OR_RETURN(r.delivery_tag, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(r.multiple, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicReject(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicReject r;
  PL_ASSIGN_OR_RETURN(r.delivery_tag, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(r.requeue, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicRecoverAsync(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicRecoverAsync r;
  PL_ASSIGN_OR_RETURN(r.requeue, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicRecover(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicRecover r;
  PL_ASSIGN_OR_RETURN(r.requeue, ExtractNthBit(decoder, 0));
  decoder->ExtractChar<uint8_t>();
  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPBasicRecoverOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicRecoverOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPBasicContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPBasicContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  if ((property_flags >> 15) & 1) {
    PL_ASSIGN_OR_RETURN(r.content_type, ExtractShortString(decoder));
  }

  if ((property_flags >> 14) & 1) {
    PL_ASSIGN_OR_RETURN(r.content_encoding, ExtractShortString(decoder));
  }

  if ((property_flags >> 13) & 1) {
    PL_ASSIGN_OR_RETURN(r.headers, ExtractLongString(decoder));
  }

  if ((property_flags >> 12) & 1) {
    PL_ASSIGN_OR_RETURN(r.delivery_mode, decoder->ExtractChar<uint8_t>());
  }

  if ((property_flags >> 11) & 1) {
    PL_ASSIGN_OR_RETURN(r.priority, decoder->ExtractChar<uint8_t>());
  }

  if ((property_flags >> 10) & 1) {
    PL_ASSIGN_OR_RETURN(r.correlation_id, ExtractShortString(decoder));
  }

  if ((property_flags >> 9) & 1) {
    PL_ASSIGN_OR_RETURN(r.reply_to, ExtractShortString(decoder));
  }

  if ((property_flags >> 8) & 1) {
    PL_ASSIGN_OR_RETURN(r.expiration, ExtractShortString(decoder));
  }

  if ((property_flags >> 7) & 1) {
    PL_ASSIGN_OR_RETURN(r.message_id, ExtractShortString(decoder));
  }

  if ((property_flags >> 6) & 1) {
    PL_ASSIGN_OR_RETURN(r.timestamp, decoder->ExtractInt<time_t>());
  }

  if ((property_flags >> 5) & 1) {
    PL_ASSIGN_OR_RETURN(r.type, ExtractShortString(decoder));
  }

  if ((property_flags >> 4) & 1) {
    PL_ASSIGN_OR_RETURN(r.user_id, ExtractShortString(decoder));
  }

  if ((property_flags >> 3) & 1) {
    PL_ASSIGN_OR_RETURN(r.app_id, ExtractShortString(decoder));
  }

  if ((property_flags >> 2) & 1) {
    PL_ASSIGN_OR_RETURN(r.reserved, ExtractShortString(decoder));
  }

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ExtractAMQPTxSelect([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxSelect r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxSelectOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxSelectOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxCommit([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxCommit r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxCommitOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxCommitOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxRollback([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxRollback r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxRollbackOk([[maybe_unused]] BinaryDecoder* decoder, Frame* frame) {
  AMQPTxRollbackOk r;

  frame->msg = ToString(r);
  frame->synchronous = 1;
  return Status::OK();
}

Status ExtractAMQPTxContentHeader(BinaryDecoder* decoder, Frame* frame) {
  AMQPTxContentHeader r;
  PL_ASSIGN_OR_RETURN(r.body_size, decoder->ExtractInt<uint64_t>());
  PL_ASSIGN_OR_RETURN(uint16_t property_flags, decoder->ExtractInt<uint16_t>());
  r.property_flags = property_flags;

  frame->msg = ToString(r);
  frame->synchronous = 0;
  return Status::OK();
}

Status ProcessContentHeader(BinaryDecoder* decoder, Frame* req) {
  PL_ASSIGN_OR_RETURN(uint16_t class_id, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(uint16_t weight, decoder->ExtractInt<uint16_t>());
  req->class_id = class_id;

  if (weight != 0) {
    return error::Internal("AMQP content header weight should be 0");
  }
  switch (static_cast<AMQPClasses>(class_id)) {
    case AMQPClasses::kConnection:
      return ExtractAMQPConnectionContentHeader(decoder, req);

    case AMQPClasses::kChannel:
      return ExtractAMQPChannelContentHeader(decoder, req);

    case AMQPClasses::kExchange:
      return ExtractAMQPExchangeContentHeader(decoder, req);

    case AMQPClasses::kQueue:
      return ExtractAMQPQueueContentHeader(decoder, req);

    case AMQPClasses::kBasic:
      return ExtractAMQPBasicContentHeader(decoder, req);

    case AMQPClasses::kTx:
      return ExtractAMQPTxContentHeader(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Unparsed frame method class $0", class_id);
  }
  return Status::OK();
}

Status ProcessConnection(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPConnectionMethods>(method_id)) {
    case AMQPConnectionMethods::kAMQPConnectionStart:
      return ExtractAMQPConnectionStart(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionStartOk:
      return ExtractAMQPConnectionStartOk(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionSecure:
      return ExtractAMQPConnectionSecure(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionSecureOk:
      return ExtractAMQPConnectionSecureOk(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionTune:
      return ExtractAMQPConnectionTune(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionTuneOk:
      return ExtractAMQPConnectionTuneOk(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionOpen:
      return ExtractAMQPConnectionOpen(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionOpenOk:
      return ExtractAMQPConnectionOpenOk(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionClose:
      return ExtractAMQPConnectionClose(decoder, req);

    case AMQPConnectionMethods::kAMQPConnectionCloseOk:
      return ExtractAMQPConnectionCloseOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Connection frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessChannel(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPChannelMethods>(method_id)) {
    case AMQPChannelMethods::kAMQPChannelOpen:
      return ExtractAMQPChannelOpen(decoder, req);

    case AMQPChannelMethods::kAMQPChannelOpenOk:
      return ExtractAMQPChannelOpenOk(decoder, req);

    case AMQPChannelMethods::kAMQPChannelFlow:
      return ExtractAMQPChannelFlow(decoder, req);

    case AMQPChannelMethods::kAMQPChannelFlowOk:
      return ExtractAMQPChannelFlowOk(decoder, req);

    case AMQPChannelMethods::kAMQPChannelClose:
      return ExtractAMQPChannelClose(decoder, req);

    case AMQPChannelMethods::kAMQPChannelCloseOk:
      return ExtractAMQPChannelCloseOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Channel frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessExchange(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPExchangeMethods>(method_id)) {
    case AMQPExchangeMethods::kAMQPExchangeDeclare:
      return ExtractAMQPExchangeDeclare(decoder, req);

    case AMQPExchangeMethods::kAMQPExchangeDeclareOk:
      return ExtractAMQPExchangeDeclareOk(decoder, req);

    case AMQPExchangeMethods::kAMQPExchangeDelete:
      return ExtractAMQPExchangeDelete(decoder, req);

    case AMQPExchangeMethods::kAMQPExchangeDeleteOk:
      return ExtractAMQPExchangeDeleteOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Exchange frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessQueue(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPQueueMethods>(method_id)) {
    case AMQPQueueMethods::kAMQPQueueDeclare:
      return ExtractAMQPQueueDeclare(decoder, req);

    case AMQPQueueMethods::kAMQPQueueDeclareOk:
      return ExtractAMQPQueueDeclareOk(decoder, req);

    case AMQPQueueMethods::kAMQPQueueBind:
      return ExtractAMQPQueueBind(decoder, req);

    case AMQPQueueMethods::kAMQPQueueBindOk:
      return ExtractAMQPQueueBindOk(decoder, req);

    case AMQPQueueMethods::kAMQPQueueUnbind:
      return ExtractAMQPQueueUnbind(decoder, req);

    case AMQPQueueMethods::kAMQPQueueUnbindOk:
      return ExtractAMQPQueueUnbindOk(decoder, req);

    case AMQPQueueMethods::kAMQPQueuePurge:
      return ExtractAMQPQueuePurge(decoder, req);

    case AMQPQueueMethods::kAMQPQueuePurgeOk:
      return ExtractAMQPQueuePurgeOk(decoder, req);

    case AMQPQueueMethods::kAMQPQueueDelete:
      return ExtractAMQPQueueDelete(decoder, req);

    case AMQPQueueMethods::kAMQPQueueDeleteOk:
      return ExtractAMQPQueueDeleteOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Queue frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessBasic(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPBasicMethods>(method_id)) {
    case AMQPBasicMethods::kAMQPBasicQos:
      return ExtractAMQPBasicQos(decoder, req);

    case AMQPBasicMethods::kAMQPBasicQosOk:
      return ExtractAMQPBasicQosOk(decoder, req);

    case AMQPBasicMethods::kAMQPBasicConsume:
      return ExtractAMQPBasicConsume(decoder, req);

    case AMQPBasicMethods::kAMQPBasicConsumeOk:
      return ExtractAMQPBasicConsumeOk(decoder, req);

    case AMQPBasicMethods::kAMQPBasicCancel:
      return ExtractAMQPBasicCancel(decoder, req);

    case AMQPBasicMethods::kAMQPBasicCancelOk:
      return ExtractAMQPBasicCancelOk(decoder, req);

    case AMQPBasicMethods::kAMQPBasicPublish:
      return ExtractAMQPBasicPublish(decoder, req);

    case AMQPBasicMethods::kAMQPBasicReturn:
      return ExtractAMQPBasicReturn(decoder, req);

    case AMQPBasicMethods::kAMQPBasicDeliver:
      return ExtractAMQPBasicDeliver(decoder, req);

    case AMQPBasicMethods::kAMQPBasicGet:
      return ExtractAMQPBasicGet(decoder, req);

    case AMQPBasicMethods::kAMQPBasicGetOk:
      return ExtractAMQPBasicGetOk(decoder, req);

    case AMQPBasicMethods::kAMQPBasicGetEmpty:
      return ExtractAMQPBasicGetEmpty(decoder, req);

    case AMQPBasicMethods::kAMQPBasicAck:
      return ExtractAMQPBasicAck(decoder, req);

    case AMQPBasicMethods::kAMQPBasicReject:
      return ExtractAMQPBasicReject(decoder, req);

    case AMQPBasicMethods::kAMQPBasicRecoverAsync:
      return ExtractAMQPBasicRecoverAsync(decoder, req);

    case AMQPBasicMethods::kAMQPBasicRecover:
      return ExtractAMQPBasicRecover(decoder, req);

    case AMQPBasicMethods::kAMQPBasicRecoverOk:
      return ExtractAMQPBasicRecoverOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Basic frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessTx(BinaryDecoder* decoder, Frame* req, uint16_t method_id) {
  switch (static_cast<AMQPTxMethods>(method_id)) {
    case AMQPTxMethods::kAMQPTxSelect:
      return ExtractAMQPTxSelect(decoder, req);

    case AMQPTxMethods::kAMQPTxSelectOk:
      return ExtractAMQPTxSelectOk(decoder, req);

    case AMQPTxMethods::kAMQPTxCommit:
      return ExtractAMQPTxCommit(decoder, req);

    case AMQPTxMethods::kAMQPTxCommitOk:
      return ExtractAMQPTxCommitOk(decoder, req);

    case AMQPTxMethods::kAMQPTxRollback:
      return ExtractAMQPTxRollback(decoder, req);

    case AMQPTxMethods::kAMQPTxRollbackOk:
      return ExtractAMQPTxRollbackOk(decoder, req);

    default:
      VLOG(1) << absl::Substitute("Invalid Tx frame method $0", method_id);
  }
  return Status::OK();
}

Status ProcessFrameMethod(BinaryDecoder* decoder, Frame* req) {
  PL_ASSIGN_OR_RETURN(uint16_t class_id, decoder->ExtractInt<uint16_t>());
  PL_ASSIGN_OR_RETURN(uint16_t method_id, decoder->ExtractInt<uint16_t>());

  req->class_id = class_id;
  req->method_id = method_id;

  switch (static_cast<AMQPClasses>(class_id)) {
    case AMQPClasses::kConnection:
      return ProcessConnection(decoder, req, method_id);

    case AMQPClasses::kChannel:
      return ProcessChannel(decoder, req, method_id);

    case AMQPClasses::kExchange:
      return ProcessExchange(decoder, req, method_id);

    case AMQPClasses::kQueue:
      return ProcessQueue(decoder, req, method_id);

    case AMQPClasses::kBasic:
      return ProcessBasic(decoder, req, method_id);

    case AMQPClasses::kTx:
      return ProcessTx(decoder, req, method_id);

    default:
      VLOG(1) << absl::Substitute("Unparsed frame method class $0 method $1", class_id, method_id);
  }
  return Status::OK();
}

Status ProcessReq(Frame* req) {
  BinaryDecoder decoder(req->msg);
  // Extracts api_key, api_version, and correlation_id.
  AMQPFrameTypes amqp_frame_type = static_cast<AMQPFrameTypes>(req->frame_type);
  switch (amqp_frame_type) {
    case AMQPFrameTypes::kFrameHeader:
      return ProcessContentHeader(&decoder, req);
    case AMQPFrameTypes::kFrameBody:
      req->msg = "";
      break;  // Ignore bytes in content body since length already provided by
              // header
    case AMQPFrameTypes::kFrameHeartbeat:
      req->msg = "";
      break;  // Heartbeat frames have no body or length
    case AMQPFrameTypes::kFrameMethod:
      return ProcessFrameMethod(&decoder, req);
    default:
      VLOG(1) << absl::Substitute("Unparsed frame $0", req->frame_type);
  }
  return Status::OK();
}

}  // namespace amqp
}  // namespace protocols
}  // namespace stirling
}  // namespace px
