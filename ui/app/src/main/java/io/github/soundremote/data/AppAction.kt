package io.github.soundremote.data

import androidx.annotation.StringRes
import io.github.soundremote.R

internal enum class AppAction(
    val id: Int,
    @StringRes
    val nameStringId: Int,
) {
    MUTE(1, R.string.app_action_mute),
    UNMUTE(2, R.string.app_action_unmute),
    CONNECT(3, R.string.app_action_connect),
    DISCONNECT(4, R.string.app_action_disconnect);

    companion object {
        /**
         * Get enum entry by its id.
         * @throws [NoSuchElementException] if no entry with such id is found.
         */
        fun getById(id: Int): AppAction {
            return entries.first { it.id == id }
        }
    }
}
