package io.github.soundremote.data

import android.os.Parcelable
import androidx.room.ColumnInfo
import androidx.room.Ignore
import kotlinx.parcelize.Parcelize

data class ActionData(
    @ColumnInfo(name = COLUMN_TYPE, defaultValue = "0")
    var actionType: Int,

    @ColumnInfo(name = COLUMN_ID)
    var actionId: Int,
) {
    @Ignore
    constructor(actionType: ActionType, actionId: Int) : this(actionType.id, actionId)

    companion object {
        const val COLUMN_TYPE = "action_type"
        const val COLUMN_ID = "action_id"
    }
}

@Parcelize
data class Action(val type: ActionType, val id: Int) : Parcelable
