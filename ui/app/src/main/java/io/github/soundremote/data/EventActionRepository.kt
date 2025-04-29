package io.github.soundremote.data

import kotlinx.coroutines.flow.Flow

interface EventActionRepository {
    suspend fun getById(id: Int): EventAction?

    suspend fun insert(eventAction: EventAction)

    suspend fun update(eventAction: EventAction): Int

    suspend fun deleteById(id: Int)

    fun getAll(): Flow<List<EventAction>>

    fun getShakeEventFlow(): Flow<EventAction?>
}
