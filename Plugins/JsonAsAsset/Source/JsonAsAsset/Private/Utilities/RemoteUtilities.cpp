/* Copyright JsonAsAsset Contributors 2024-2025 */

#include "Utilities/RemoteUtilities.h"

#include "HttpManager.h"
#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

#if ENGINE_UE5
TSharedPtr<IHttpResponse> FRemoteUtilities::ExecuteRequestSync(TSharedRef<IHttpRequest> HttpRequest, float LoopDelay)
#else
TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> FRemoteUtilities::ExecuteRequestSync(const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> &HttpRequest, float LoopDelay)
#endif
{
	return nullptr;
}
