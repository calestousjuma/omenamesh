package io.github.soundremote.ui.settings

import androidx.activity.ComponentActivity
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.assertIsNotSelected
import androidx.compose.ui.test.assertTextContains
import androidx.compose.ui.test.hasAnyAncestor
import androidx.compose.ui.test.hasText
import androidx.compose.ui.test.isDialog
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithContentDescription
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performTextClearance
import androidx.compose.ui.test.performTextInput
import io.github.soundremote.R
import io.github.soundremote.stringResource
import io.github.soundremote.ui.theme.SoundRemoteTheme
import io.github.soundremote.util.Net
import io.github.soundremote.util.Net.COMPRESSION_320
import io.github.soundremote.util.Net.COMPRESSION_NONE
import io.github.soundremote.util.TestTag
import org.junit.Assert.assertEquals
import org.junit.Rule
import org.junit.Test

internal class SettingsScreenTest {
    @get:Rule
    val composeTestRule = createAndroidComposeRule<ComponentActivity>()

    private val navigateUp by composeTestRule.stringResource(R.string.navigate_up)
    private val audioCompression by composeTestRule.stringResource(R.string.pref_compression_title)
    private val clientPort by composeTestRule.stringResource(R.string.pref_client_port_title)
    private val serverPort by composeTestRule.stringResource(R.string.pref_server_port_title)
    private val compressionNone by composeTestRule.stringResource(R.string.compression_none)
    private val compression320 by composeTestRule.stringResource(R.string.compression_320)
    private val ok by composeTestRule.stringResource(android.R.string.ok)

    // Settings screen should contain navigate up arrow
    @Test
    fun navigateUp_exist() {
        composeTestRule.setContent {
            CreateSettingsScreen()
        }

        composeTestRule.onNodeWithContentDescription(navigateUp).assertIsDisplayed()
    }

    // All settings are displayed
    @Test
    fun allSettings_areDisplayed() {
        composeTestRule.setContent {
            CreateSettingsScreen()
        }

        composeTestRule.onNodeWithText(audioCompression).assertIsDisplayed()
        composeTestRule.onNodeWithText(clientPort).assertIsDisplayed()
        composeTestRule.onNodeWithText(serverPort).assertIsDisplayed()
    }

    // Current value display

    // Audio compression preference displays the current value if compression is on
    @Test
    fun audioCompressionOn_displaysCurrentValue() {
        @Net.Compression val value = COMPRESSION_320
        composeTestRule.setContent {
            CreateSettingsScreen(settings = SettingsUIState(audioCompression = value))
        }

        composeTestRule.onNodeWithText(this.audioCompression)
            .assertTextContains(compression320, true)
    }

    // Audio compression preference displays the current value if compression is off
    @Test
    fun audioCompressionOff_displaysCurrentValue() {
        @Net.Compression val value = COMPRESSION_NONE
        composeTestRule.setContent {
            CreateSettingsScreen(settings = SettingsUIState(audioCompression = value))
        }

        composeTestRule.onNodeWithText(audioCompression)
            .assertTextContains(compressionNone, true)
    }

    // Server port preference displays the current value
    @Test
    fun serverPort_displaysCurrentValue() {
        val value = 6789
        composeTestRule.setContent {
            CreateSettingsScreen(settings = SettingsUIState(serverPort = value))
        }

        composeTestRule.onNodeWithText(this.serverPort)
            .assertTextContains("$value", true)
    }

    // Client port preference displays the current value
    @Test
    fun clientPort_displaysCurrentValue() {
        val value = 5678
        composeTestRule.setContent {
            CreateSettingsScreen(settings = SettingsUIState(clientPort = value))
        }

        composeTestRule.onNodeWithText(this.clientPort)
            .assertTextContains("$value", true)
    }

    // On click

    // Click on audio compression preference should show selection dialog
    @Test
    fun audioCompression_onClick_showsSelectDialog() {
        composeTestRule.setContent {
            CreateSettingsScreen()
        }

        composeTestRule.onNodeWithText(audioCompression).performClick()

        composeTestRule.onNode(
            hasText(audioCompression)
                    and hasAnyAncestor(isDialog())
        ).assertIsDisplayed()
    }

    // Click on server port preference should show edit dialog
    @Test
    fun serverPort_onClick_showsSelectDialog() {
        composeTestRule.setContent {
            CreateSettingsScreen()
        }
        composeTestRule.onNodeWithText(serverPort).performClick()

        composeTestRule.onNode(
            hasText(serverPort)
                    and hasAnyAncestor(isDialog())
        ).assertIsDisplayed()
    }

    // Click on client port preference should show edit dialog
    @Test
    fun clientPort_onClick_showsSelectDialog() {
        composeTestRule.setContent {
            CreateSettingsScreen()
        }

        composeTestRule.onNodeWithText(clientPort).performClick()

        composeTestRule.onNode(
            hasText(clientPort)
                    and hasAnyAncestor(isDialog())
        ).assertIsDisplayed()
    }

    // Update

    // Clicking on an audio compression option in the select dialog invokes update callback
    @Test
    fun audioCompressionOption_onClick_invokesCallback() {
        var actual = 0
        composeTestRule.setContent {
            CreateSettingsScreen(onSetAudioCompression = { actual = it })
        }

        composeTestRule.apply {
            onNodeWithText(audioCompression).performClick()
            onNodeWithText(compression320).assertIsNotSelected()
            onNodeWithText(compression320).performClick()
        }

        assertEquals(COMPRESSION_320, actual)
    }

    // Clicking confirm button in edit server port dialog invokes update callback with correct value
    @Test
    fun serverPortEditDialog_onOk_invokesCallback() {
        var actual = 0
        val current = 10_000
        composeTestRule.setContent {
            CreateSettingsScreen(
                settings = SettingsUIState(serverPort = current),
                onSetServerPort = { actual = it })
        }

        val expected = 22_322
        composeTestRule.apply {
            onNodeWithText(serverPort).performClick()
            onNodeWithTag(TestTag.INPUT_FIELD).performTextClearance()
            onNodeWithTag(TestTag.INPUT_FIELD).performTextInput("$expected")
            onNodeWithText(ok).performClick()
        }

        assertEquals(expected, actual)
    }

    // Clicking confirm button in edit client port dialog invokes update callback with correct value
    @Test
    fun clientPortEditDialog_onOk_invokesCallback() {
        var actual = 0
        val current = 10_000
        composeTestRule.setContent {
            CreateSettingsScreen(
                settings = SettingsUIState(clientPort = current),
                onSetClientPort = { actual = it })
        }

        val expected = 22_322
        composeTestRule.apply {
            onNodeWithText(clientPort).performClick()
            onNodeWithTag(TestTag.INPUT_FIELD).performTextClearance()
            onNodeWithTag(TestTag.INPUT_FIELD).performTextInput("$expected")
            onNodeWithText(ok).performClick()
        }

        assertEquals(expected, actual)
    }

    @Suppress("TestFunctionName")
    @Composable
    private fun CreateSettingsScreen(
        modifier: Modifier = Modifier,
        settings: SettingsUIState = SettingsUIState(),
        onSetServerPort: (Int) -> Unit = {},
        onSetClientPort: (Int) -> Unit = {},
        onSetAudioCompression: (Int) -> Unit = {},
        onNavigateUp: () -> Unit = {},
    ) {
        SoundRemoteTheme {
            SettingsScreen(
                settings = settings,
                onSetServerPort = onSetServerPort,
                onSetClientPort = onSetClientPort,
                onSetAudioCompression = onSetAudioCompression,
                onNavigateUp = onNavigateUp,
                modifier = modifier,
            )
        }
    }
}
