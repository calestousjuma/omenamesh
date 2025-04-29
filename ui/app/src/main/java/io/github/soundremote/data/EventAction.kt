package io.github.soundremote.data

import android.provider.BaseColumns
import androidx.room.ColumnInfo
import androidx.room.Embedded
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = EventAction.TABLE_NAME)
data class EventAction(
    @ColumnInfo(name = COLUMN_ID)
    @PrimaryKey
    var eventId: Int,

    @Embedded
    var action: ActionData
) {
    companion object {
        const val TABLE_NAME = "event_action"
        const val COLUMN_ID = BaseColumns._ID
    }
}
