package io.github.soundremote.ui

import io.github.soundremote.MainDispatcherExtension
import io.github.soundremote.data.preferences.TestPreferencesRepository
import io.github.soundremote.ui.settings.SettingsViewModel
import io.github.soundremote.util.DEFAULT_AUDIO_COMPRESSION
import io.github.soundremote.util.DEFAULT_CLIENT_PORT
import io.github.soundremote.util.DEFAULT_SERVER_PORT
import io.github.soundremote.util.Net.COMPRESSION_320
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.launch
import kotlinx.coroutines.test.UnconfinedTestDispatcher
import kotlinx.coroutines.test.runTest
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.DisplayName
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.extension.ExtendWith

@OptIn(ExperimentalCoroutinesApi::class)
@ExtendWith(MainDispatcherExtension::class)
@DisplayName("SettingsViewModel")
class SettingsViewModelTest {
    private val preferencesRepository = TestPreferencesRepository()

    private lateinit var viewModel: SettingsViewModel

    @BeforeEach
    fun setup() {
        viewModel = SettingsViewModel(preferencesRepository)
    }

    @Test
    @DisplayName("Setting audio compression updates settings")
    fun audioCompression_changes_settingsStateUpdates() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.settings.collect {}
        }

        assertEquals(DEFAULT_AUDIO_COMPRESSION, viewModel.settings.value.audioCompression)
        val expected = COMPRESSION_320

        preferencesRepository.setAudioCompression(expected)

        assertEquals(expected, viewModel.settings.value.audioCompression)

        collectJob.cancel()
    }

    @Test
    @DisplayName("Setting client port updates settings")
    fun clientPort_changes_settingsStateUpdates() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.settings.collect {}
        }

        assertEquals(DEFAULT_CLIENT_PORT, viewModel.settings.value.clientPort)
        val expected = 33333

        preferencesRepository.setClientPort(expected)

        assertEquals(expected, viewModel.settings.value.clientPort)

        collectJob.cancel()
    }

    @Test
    @DisplayName("Setting server port updates settings")
    fun serverPort_changes_settingsStateUpdates() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.settings.collect {}
        }

        assertEquals(DEFAULT_SERVER_PORT, viewModel.settings.value.serverPort)
        val expected = 44444

        preferencesRepository.setServerPort(expected)

        assertEquals(expected, viewModel.settings.value.serverPort)

        collectJob.cancel()
    }
}
