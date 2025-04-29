package io.github.soundremote.ui.settings

import android.content.res.Configuration
import androidx.compose.foundation.layout.Column
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import io.github.soundremote.R
import io.github.soundremote.ui.theme.SoundRemoteTheme
import io.github.soundremote.ui.components.NavigateUpButton
import io.github.soundremote.util.DEFAULT_CLIENT_PORT
import io.github.soundremote.util.DEFAULT_SERVER_PORT
import io.github.soundremote.util.Net

@OptIn(ExperimentalMaterial3Api::class)
@Composable
internal fun SettingsScreen(
    settings: SettingsUIState,
    onSetServerPort: (Int) -> Unit,
    onSetClientPort: (Int) -> Unit,
    onSetAudioCompression: (Int) -> Unit,
    onNavigateUp: () -> Unit,
    modifier: Modifier = Modifier,
) {
    val validPorts = 1024..49151
    val compressionOptions = remember {
        compressionOptions()
    }
    val compressionSummaryId = remember(settings.audioCompression) {
        compressionOptions.find { it.value == settings.audioCompression }?.textStringId
    }

    Column(modifier) {
        TopAppBar(
            title = { Text(stringResource(R.string.settings_title)) },
            navigationIcon = { NavigateUpButton(onNavigateUp) },
        )
        SelectPreference(
            title = stringResource(R.string.pref_compression_title),
            summary = if (compressionSummaryId == null) {
                ""
            } else {
                stringResource(compressionSummaryId)
            },
            options = compressionOptions,
            selected = settings.audioCompression,
            onSelect = onSetAudioCompression,
        )
        IntPreference(
            title = stringResource(R.string.pref_server_port_title),
            summary = stringResource(R.string.pref_server_port_summary),
            value = settings.serverPort,
            onUpdate = { onSetServerPort(it) },
            validValues = validPorts,
            defaultValue = DEFAULT_SERVER_PORT,
        )
        IntPreference(
            title = stringResource(R.string.pref_client_port_title),
            summary = stringResource(R.string.pref_client_port_summary),
            value = settings.clientPort,
            onUpdate = { onSetClientPort(it) },
            validValues = validPorts,
            defaultValue = DEFAULT_CLIENT_PORT,
        )
    }
}

private fun compressionOptions(): List<SelectableOption<Int>> = listOf(
    SelectableOption(Net.COMPRESSION_NONE, R.string.compression_none),
    SelectableOption(Net.COMPRESSION_64, R.string.compression_64),
    SelectableOption(Net.COMPRESSION_128, R.string.compression_128),
    SelectableOption(Net.COMPRESSION_192, R.string.compression_192),
    SelectableOption(Net.COMPRESSION_256, R.string.compression_256),
    SelectableOption(Net.COMPRESSION_320, R.string.compression_320),
)

@Preview(showBackground = true, uiMode = Configuration.UI_MODE_NIGHT_NO, name = "Light")
@Preview(showBackground = true, uiMode = Configuration.UI_MODE_NIGHT_YES, name = "Dark")
@Composable
private fun SettingsScreenPreview() {
    SoundRemoteTheme {
        SettingsScreen(
            settings = SettingsUIState(1234, 5678, 0),
            onSetClientPort = {},
            onSetServerPort = {},
            onSetAudioCompression = {},
            onNavigateUp = {},
        )
    }
}
