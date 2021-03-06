/*
 *  Copyright 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef PC_TEST_MOCK_WEBRTCSESSION_H_
#define PC_TEST_MOCK_WEBRTCSESSION_H_

#include <memory>
#include <string>

#include "pc/webrtcsession.h"
#include "media/sctp/sctptransportinternal.h"
#include "test/gmock.h"

namespace webrtc {

class MockWebRtcSession : public webrtc::WebRtcSession {
 public:
  // TODO(nisse): Valid overrides commented out, because the gmock
  // methods don't use any override declarations, and we want to avoid
  // warnings from -Winconsistent-missing-override. See
  // http://crbug.com/428099.
  explicit MockWebRtcSession(cricket::ChannelManager* channel_manager,
                             const cricket::MediaConfig& media_config)
      : WebRtcSession(nullptr /* Call */,
            channel_manager,
            media_config,
            nullptr, // event_log
            rtc::Thread::Current(),
            rtc::Thread::Current(),
            rtc::Thread::Current(),
            nullptr,
            std::unique_ptr<cricket::TransportController>(
                new cricket::TransportController(
                    rtc::Thread::Current(),
                    rtc::Thread::Current(),
                    nullptr,
                    /*redetermine_role_on_ice_restart=*/true,
                    rtc::CryptoOptions())),
            std::unique_ptr<cricket::SctpTransportInternalFactory>()) {}
  MOCK_METHOD0(voice_channel, cricket::VoiceChannel*());
  MOCK_METHOD0(video_channel, cricket::VideoChannel*());
  // Libjingle uses "local" for a outgoing track, and "remote" for a incoming
  // track.
  MOCK_METHOD2(GetLocalTrackIdBySsrc, bool(uint32_t, std::string*));
  MOCK_METHOD2(GetRemoteTrackIdBySsrc, bool(uint32_t, std::string*));
  MOCK_METHOD0(GetCallStats, Call::Stats());
  MOCK_METHOD1(GetStats,
               std::unique_ptr<SessionStats>(const ChannelNamePairs&));
  MOCK_METHOD2(GetLocalCertificate,
               bool(const std::string& transport_name,
                    rtc::scoped_refptr<rtc::RTCCertificate>* certificate));

  // Workaround for gmock's inability to cope with move-only return values.
  std::unique_ptr<rtc::SSLCertificate> GetRemoteSSLCertificate(
      const std::string& transport_name) /* override */ {
    return std::unique_ptr<rtc::SSLCertificate>(
        GetRemoteSSLCertificate_ReturnsRawPointer(transport_name));
  }
  MOCK_METHOD1(GetRemoteSSLCertificate_ReturnsRawPointer,
               rtc::SSLCertificate*(const std::string& transport_name));
};

}  // namespace webrtc

#endif  // PC_TEST_MOCK_WEBRTCSESSION_H_
