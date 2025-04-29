package io.github.soundremote.data.room

import androidx.room.Dao
import androidx.room.Query
import io.github.soundremote.data.EventAction
import kotlinx.coroutines.flow.Flow

@Dao
interface EventActionDao : BaseDao<EventAction> {

    @Query("SELECT * FROM ${EventAction.TABLE_NAME} WHERE ${EventAction.COLUMN_ID} = :id")
    suspend fun getById(id: Int): EventAction?

    @Query("DELETE FROM ${EventAction.TABLE_NAME} WHERE ${EventAction.COLUMN_ID} = :id")
    suspend fun deleteById(id: Int)

    @Query("SELECT * FROM ${EventAction.TABLE_NAME}")
    fun getAll(): Flow<List<EventAction>>

    @Query("SELECT * FROM ${EventAction.TABLE_NAME} WHERE ${EventAction.COLUMN_ID} = :eventId")
    fun getEventActionFlow(eventId: Int): Flow<EventAction?>
}
