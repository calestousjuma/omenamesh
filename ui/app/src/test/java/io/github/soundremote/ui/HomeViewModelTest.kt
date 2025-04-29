package io.github.soundremote.ui

import io.github.soundremote.MainDispatcherExtension
import io.github.soundremote.data.TestHotkeyRepository
import io.github.soundremote.data.preferences.TestPreferencesRepository
import io.github.soundremote.getHotkey
import io.github.soundremote.service.ServiceState
import io.github.soundremote.service.TestServiceManager
import io.github.soundremote.ui.home.HomeViewModel
import io.github.soundremote.util.ConnectionStatus
import io.github.soundremote.util.Key
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.launch
import kotlinx.coroutines.test.UnconfinedTestDispatcher
import kotlinx.coroutines.test.runTest
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertNotNull
import org.junit.jupiter.api.Assertions.assertNull
import org.junit.jupiter.api.Assertions.assertTrue
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.DisplayName
import org.junit.jupiter.api.Nested
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.extension.ExtendWith

@OptIn(ExperimentalCoroutinesApi::class)
@ExtendWith(MainDispatcherExtension::class)
@DisplayName("HomeViewModel")
class HomeViewModelTest {
    private var preferencesRepository = TestPreferencesRepository()
    private var hotkeyRepository = TestHotkeyRepository()
    private var serviceManager = TestServiceManager()
    private lateinit var viewModel: HomeViewModel

    @BeforeEach
    fun setup() {
        viewModel = HomeViewModel(preferencesRepository, hotkeyRepository, serviceManager)
    }

    @Nested
    @DisplayName("connect")
    inner class Connect {
        @Test
        fun `with valid address changes status to CONNECTED`() = runTest {
            val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
                viewModel.homeUIState.collect {}
            }
            hotkeyRepository.setHotkeys(emptyList())

            viewModel.connect("123.45.67.89")

            val actualStatus = viewModel.homeUIState.value.connectionStatus
            assertEquals(ConnectionStatus.CONNECTED, actualStatus)

            collectJob.cancel()
        }

        @Test
        fun `with invalid address changes messageState`() = runTest {
            val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
                viewModel.homeUIState.collect {}
            }
            hotkeyRepository.setHotkeys(emptyList())
            assertNull(viewModel.messageState)

            viewModel.connect("Invalid address")

            val actualStatus = viewModel.homeUIState.value.connectionStatus
            assertEquals(ConnectionStatus.DISCONNECTED, actualStatus)
            val actualMessage = viewModel.messageState
            assertNotNull(actualMessage)

            collectJob.cancel()
        }

        @Test
        fun `sets address as current server address`() = runTest {
            val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
                viewModel.homeUIState.collect {}
            }
            val expected = "123.45.67.89"

            viewModel.connect("192.168.0.1")
            viewModel.connect("192.168.0.2")
            viewModel.connect(expected)

            val actual = viewModel.homeUIState.value.serverAddress
            assertEquals(expected, actual)

            collectJob.cancel()
        }

        @Test
        fun `adds address to 'Recent servers' list`() = runTest {
            val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
                viewModel.homeUIState.collect {}
            }
            val expected = "123.45.67.89"

            viewModel.connect(expected)

            val actual = viewModel.homeUIState.value.recentServersAddresses.last()
            assertEquals(expected, actual)

            collectJob.cancel()
        }
    }

    @Test
    @DisplayName("messageShown() sets messageState to null")
    fun messageShown_nullsMessageState() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.homeUIState.collect {}
        }
        hotkeyRepository.setHotkeys(emptyList())

        viewModel.connect("Invalid address")
        assertNotNull(viewModel.messageState)

        viewModel.messageShown()

        assertNull(viewModel.messageState)

        collectJob.cancel()
    }

    @Test
    @DisplayName("disconnect() changes status to DISCONNECTED")
    fun disconnect_changesStatus() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.homeUIState.collect {}
        }
        hotkeyRepository.setHotkeys(emptyList())

        serviceManager.setServiceState(ServiceState(ConnectionStatus.CONNECTED))

        viewModel.disconnect()

        val actualStatus = viewModel.homeUIState.value.connectionStatus
        assertEquals(ConnectionStatus.DISCONNECTED, actualStatus)

        collectJob.cancel()
    }

    @Test
    @DisplayName("setMuted() changes muted status")
    fun setMuted_changesMutedStatus() = runTest {
        val collectJob = backgroundScope.launch(UnconfinedTestDispatcher(testScheduler)) {
            viewModel.homeUIState.collect {}
        }
        hotkeyRepository.setHotkeys(emptyList())

        serviceManager.setServiceState(ServiceState(isMuted = false))

        viewModel.setMuted(true)

        val actualStatus = viewModel.homeUIState.value.isMuted
        assertTrue(actualStatus)

        collectJob.cancel()
    }

    @Test
    @DisplayName("sendHotkey() calls ServiceManger.sendHotkey()")
    fun sendHotkey_callsService() = runTest {
        val id = 3
        val expected = getHotkey(id = id)
        hotkeyRepository.setHotkeys(listOf(expected))

        viewModel.sendHotkey(id)

        val actual = serviceManager.sentHotkey
        assertEquals(expected, actual)
    }

    @Test
    @DisplayName("sendKey() calls ServiceManger.sendKey()")
    fun sendKey_callsService() = runTest {
        val expected = Key.MEDIA_PLAY_PAUSE

        viewModel.sendKey(expected)

        val actual = serviceManager.sentKey
        assertEquals(expected, actual)
    }
}
