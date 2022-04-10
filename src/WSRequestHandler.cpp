/**
 * obs-websocket
 * Copyright (C) 2016-2017	Stéphane Lepin <stephane.lepin@gmail.com>
 * Copyright (C) 2017	Mikhail Swift <https://github.com/mikhailswift>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include <functional>

#include <obs-data.h>

#include "Config.h"
#include "Utils.h"

#include "WSRequestHandler.h"

using namespace std::placeholders;

const QHash<QString, RpcMethodHandler> WSRequestHandler::messageMap {
	{ "GetVersion", &WSRequestHandler::GetVersion },
	{ "GetAuthRequired", &WSRequestHandler::GetAuthRequired },
	{ "Authenticate", &WSRequestHandler::Authenticate },

	{ "GetStats", &WSRequestHandler::GetStats },
	{ "SetHeartbeat", &WSRequestHandler::SetHeartbeat },
	{ "GetVideoInfo", &WSRequestHandler::GetVideoInfo },
	{ "OpenProjector", &WSRequestHandler::OpenProjector },

	{ "SetFilenameFormatting", &WSRequestHandler::SetFilenameFormatting },
	{ "GetFilenameFormatting", &WSRequestHandler::GetFilenameFormatting },

	{ "BroadcastCustomMessage", &WSRequestHandler::BroadcastCustomMessage },

	{ "TriggerHotkeyByName", &WSRequestHandler::TriggerHotkeyByName },
	{ "TriggerHotkeyBySequence", &WSRequestHandler::TriggerHotkeyBySequence },

	{ "SetCurrentScene", &WSRequestHandler::SetCurrentScene },
	{ "GetCurrentScene", &WSRequestHandler::GetCurrentScene },
	{ "GetSceneList", &WSRequestHandler::GetSceneList },
	{ "CreateScene", &WSRequestHandler::CreateScene },
	{ "SetSceneTransitionOverride", &WSRequestHandler::SetSceneTransitionOverride },
	{ "RemoveSceneTransitionOverride", &WSRequestHandler::RemoveSceneTransitionOverride },
	{ "GetSceneTransitionOverride", &WSRequestHandler::GetSceneTransitionOverride },

	{ "SetSourceRender", &WSRequestHandler::SetSceneItemRender }, // Retrocompat
	{ "GetSceneItemList", &WSRequestHandler::GetSceneItemList },
	{ "SetSceneItemRender", &WSRequestHandler::SetSceneItemRender },
	{ "SetSceneItemPosition", &WSRequestHandler::SetSceneItemPosition },
	{ "SetSceneItemTransform", &WSRequestHandler::SetSceneItemTransform },
	{ "SetSceneItemCrop", &WSRequestHandler::SetSceneItemCrop },
	{ "GetSceneItemProperties", &WSRequestHandler::GetSceneItemProperties },
	{ "SetSceneItemProperties", &WSRequestHandler::SetSceneItemProperties },
	{ "ResetSceneItem", &WSRequestHandler::ResetSceneItem },
	{ "DeleteSceneItem", &WSRequestHandler::DeleteSceneItem },
	{ "AddSceneItem", &WSRequestHandler::AddSceneItem },
	{ "DuplicateSceneItem", &WSRequestHandler::DuplicateSceneItem },
	{ "ReorderSceneItems", &WSRequestHandler::ReorderSceneItems },

	{ "GetStreamingStatus", &WSRequestHandler::GetStreamingStatus },
	{ "StartStopStreaming", &WSRequestHandler::StartStopStreaming },
	{ "StartStopRecording", &WSRequestHandler::StartStopRecording },

	{ "StartStreaming", &WSRequestHandler::StartStreaming },
	{ "StopStreaming", &WSRequestHandler::StopStreaming },

	{ "GetRecordingStatus", &WSRequestHandler::GetRecordingStatus },
	{ "StartRecording", &WSRequestHandler::StartRecording },
	{ "StopRecording", &WSRequestHandler::StopRecording },
	{ "PauseRecording", &WSRequestHandler::PauseRecording },
	{ "ResumeRecording", &WSRequestHandler::ResumeRecording },

	{ "GetReplayBufferStatus", &WSRequestHandler::GetReplayBufferStatus },
	{ "StartStopReplayBuffer", &WSRequestHandler::StartStopReplayBuffer },
	{ "StartReplayBuffer", &WSRequestHandler::StartReplayBuffer },
	{ "StopReplayBuffer", &WSRequestHandler::StopReplayBuffer },
	{ "SaveReplayBuffer", &WSRequestHandler::SaveReplayBuffer },

	{ "SetRecordingFolder", &WSRequestHandler::SetRecordingFolder },
	{ "GetRecordingFolder", &WSRequestHandler::GetRecordingFolder },

	{ "GetTransitionList", &WSRequestHandler::GetTransitionList },
	{ "GetCurrentTransition", &WSRequestHandler::GetCurrentTransition },
	{ "SetCurrentTransition", &WSRequestHandler::SetCurrentTransition },
	{ "SetTransitionDuration", &WSRequestHandler::SetTransitionDuration },
	{ "GetTransitionDuration", &WSRequestHandler::GetTransitionDuration },
	{ "GetTransitionPosition", &WSRequestHandler::GetTransitionPosition },

	{ "SetVolume", &WSRequestHandler::SetVolume },
	{ "GetVolume", &WSRequestHandler::GetVolume },
	{ "ToggleMute", &WSRequestHandler::ToggleMute },
	{ "SetMute", &WSRequestHandler::SetMute },
	{ "GetMute", &WSRequestHandler::GetMute },
	{ "GetAudioActive", &WSRequestHandler::GetAudioActive },
	{ "SetSourceName", &WSRequestHandler::SetSourceName },
	{ "SetSyncOffset", &WSRequestHandler::SetSyncOffset },
	{ "GetSyncOffset", &WSRequestHandler::GetSyncOffset },
	{ "GetSpecialSources", &WSRequestHandler::GetSpecialSources },
	{ "GetSourcesList", &WSRequestHandler::GetSourcesList },
	{ "GetSourceTypesList", &WSRequestHandler::GetSourceTypesList },
	{ "GetSourceSettings", &WSRequestHandler::GetSourceSettings },
	{ "SetSourceSettings", &WSRequestHandler::SetSourceSettings },
	{ "GetAudioMonitorType", &WSRequestHandler::GetAudioMonitorType },
	{ "SetAudioMonitorType", &WSRequestHandler::SetAudioMonitorType },
	{ "TakeSourceScreenshot", &WSRequestHandler::TakeSourceScreenshot },
	{ "SubscribeAudioMeter", &WSRequestHandler::SubscribeAudioMeter },
	{ "UnsubscribeAudioMeter", &WSRequestHandler::UnsubscribeAudioMeter },

	{ "GetSourceFilters", &WSRequestHandler::GetSourceFilters },
	{ "GetSourceFilterInfo", &WSRequestHandler::GetSourceFilterInfo },
	{ "AddFilterToSource", &WSRequestHandler::AddFilterToSource },
	{ "RemoveFilterFromSource", &WSRequestHandler::RemoveFilterFromSource },
	{ "ReorderSourceFilter", &WSRequestHandler::ReorderSourceFilter },
	{ "MoveSourceFilter", &WSRequestHandler::MoveSourceFilter },
	{ "SetSourceFilterSettings", &WSRequestHandler::SetSourceFilterSettings },
	{ "SetSourceFilterVisibility", &WSRequestHandler::SetSourceFilterVisibility },

	{ "SetCurrentSceneCollection", &WSRequestHandler::SetCurrentSceneCollection },
	{ "GetCurrentSceneCollection", &WSRequestHandler::GetCurrentSceneCollection },
	{ "ListSceneCollections", &WSRequestHandler::ListSceneCollections },

	{ "SetCurrentProfile", &WSRequestHandler::SetCurrentProfile },
	{ "GetCurrentProfile", &WSRequestHandler::GetCurrentProfile },
	{ "ListProfiles", &WSRequestHandler::ListProfiles },

	{ "SetStreamSettings", &WSRequestHandler::SetStreamSettings },
	{ "GetStreamSettings", &WSRequestHandler::GetStreamSettings },
	{ "SaveStreamSettings", &WSRequestHandler::SaveStreamSettings },
#if BUILD_CAPTIONS
	{ "SendCaptions", &WSRequestHandler::SendCaptions },
#endif

	{ "GetStudioModeStatus", &WSRequestHandler::GetStudioModeStatus },
	{ "GetPreviewScene", &WSRequestHandler::GetPreviewScene },
	{ "SetPreviewScene", &WSRequestHandler::SetPreviewScene },
	{ "TransitionToProgram", &WSRequestHandler::TransitionToProgram },
	{ "EnableStudioMode", &WSRequestHandler::EnableStudioMode },
	{ "DisableStudioMode", &WSRequestHandler::DisableStudioMode },
	{ "ToggleStudioMode", &WSRequestHandler::ToggleStudioMode },

	{ "SetTextGDIPlusProperties", &WSRequestHandler::SetTextGDIPlusProperties },
	{ "GetTextGDIPlusProperties", &WSRequestHandler::GetTextGDIPlusProperties },

	{ "SetTextFreetype2Properties", &WSRequestHandler::SetTextFreetype2Properties },
	{ "GetTextFreetype2Properties", &WSRequestHandler::GetTextFreetype2Properties },

	{ "GetBrowserSourceProperties", &WSRequestHandler::GetBrowserSourceProperties },
	{ "SetBrowserSourceProperties", &WSRequestHandler::SetBrowserSourceProperties },

	{ "ListOutputs", &WSRequestHandler::ListOutputs },
	{ "GetOutputInfo", &WSRequestHandler::GetOutputInfo },
	{ "StartOutput", &WSRequestHandler::StartOutput },
	{ "StopOutput", &WSRequestHandler::StopOutput },

	{ "PlayPauseMedia", &WSRequestHandler::PlayPauseMedia },
	{ "RestartMedia", &WSRequestHandler::RestartMedia },
	{ "StopMedia", &WSRequestHandler::StopMedia },
	{ "NextMedia", &WSRequestHandler::NextMedia },
	{ "PreviousMedia", &WSRequestHandler::PreviousMedia },
	{ "GetMediaDuration", &WSRequestHandler::GetMediaDuration },
	{ "GetMediaTime", &WSRequestHandler::GetMediaTime },
	{ "SetMediaTime", &WSRequestHandler::SetMediaTime },
	{ "ScrubMedia", &WSRequestHandler::ScrubMedia },
	{ "GetMediaState", &WSRequestHandler::GetMediaState },
	{ "GetMediaSourcesList", &WSRequestHandler::GetMediaSourcesList }
};

const QSet<QString> WSRequestHandler::authNotRequired {
	"GetVersion",
	"GetAuthRequired",
	"Authenticate"
};

WSRequestHandler::WSRequestHandler(std::shared_ptr<ConnectionProperties> connProperties) :
	_connProperties(connProperties)
{
}

RpcResponse WSRequestHandler::processRequest(const RpcRequest& request){
	if (GetConfig()->AuthRequired
		&& (!authNotRequired.contains(request.methodName()))
		&& (!_connProperties->isAuthenticated()))
	{
		return RpcResponse::fail(request, "Not Authenticated");
	}

	RpcMethodHandler handlerFunc = messageMap[request.methodName()];
	if (!handlerFunc) {
		return RpcResponse::fail(request, "invalid request type");
	}

	return std::bind(handlerFunc, this, _1)(request);
}
