package io.github.soundremote.ui.settings

import androidx.compose.runtime.getValue
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.navigation.NavController
import androidx.navigation.NavGraphBuilder
import androidx.navigation.compose.composable
import kotlinx.serialization.Serializable

@Serializable
object SettingsRoute

fun NavController.navigateToSettings() {
    navigate(SettingsRoute)
}

fun NavGraphBuilder.settingsScreen(
    onNavigateUp: () -> Unit,
) {
    composable<SettingsRoute> {
        val viewModel: SettingsViewModel = hiltViewModel()
        val settings by viewModel.settings.collectAsStateWithLifecycle()
        SettingsScreen(
            settings = settings,
            onSetServerPort = { viewModel.setServerPort(it) },
            onSetClientPort = { viewModel.setClientPort(it) },
            onSetAudioCompression = { viewModel.setAudioCompression(it) },
            onNavigateUp = onNavigateUp,
        )
    }
}
