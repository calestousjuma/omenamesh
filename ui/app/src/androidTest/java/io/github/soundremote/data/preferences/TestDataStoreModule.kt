package io.github.soundremote.data.preferences

import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.PreferenceDataStoreFactory
import androidx.datastore.preferences.core.Preferences
import dagger.Module
import dagger.Provides
import dagger.hilt.components.SingletonComponent
import dagger.hilt.testing.TestInstallIn
import org.junit.rules.TemporaryFolder
import javax.inject.Singleton

@TestInstallIn(
    components = [SingletonComponent::class],
    replaces = [DataStoreModule::class],
)
@Module
object TestDataStoreModule {
    @Singleton
    @Provides
    fun providePreferencesDatastore(tmpFolder: TemporaryFolder): DataStore<Preferences> =
        PreferenceDataStoreFactory.create { tmpFolder.newFile("test.preferences_pb") }
}
