package io.github.soundremote.util

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.provider.Settings
import androidx.annotation.StringRes
import io.github.soundremote.R

internal const val ACTION_CLOSE = "io.github.soundremote.ACTION_CLOSE"

internal enum class SystemMessage(@StringRes val stringId: Int) {
    MESSAGE_CONNECT_FAILED(R.string.message_connect_failed),
    MESSAGE_ALREADY_BOUND(R.string.message_already_bound),
    MESSAGE_BIND_ERROR(R.string.message_bind_error),
    MESSAGE_DISCONNECTED(R.string.message_disconnected),
    MESSAGE_AUDIO_FOCUS_REQUEST_FAILED(R.string.message_audio_focus_request_failed),
}

// https://stackoverflow.com/questions/32822101/how-can-i-programmatically-open-the-permission-screen-for-a-specific-app-on-andr
/**
 * Shows system app info screen
 */
internal fun showAppInfo(context: Context) {
    val uri = Uri.fromParts("package", context.packageName, null)
    val intent = Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, uri)
        .addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY)
        .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
        .addFlags(Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS)
    context.startActivity(intent)
}

internal enum class AppPermission(val id: String, val nameStringId: Int) {
    Phone(android.Manifest.permission.READ_PHONE_STATE, R.string.permission_name_phone)
}

sealed interface TextValue {
    data class TextString(val str: String) : TextValue
    data class TextResource(@StringRes val strId: Int) : TextValue
}
