package io.github.soundremote.ui.about

import androidx.navigation.NavController
import androidx.navigation.NavGraphBuilder
import androidx.navigation.compose.composable
import kotlinx.serialization.Serializable

@Serializable
object AboutRoute

fun NavController.navigateToAbout() {
    navigate(AboutRoute)
}

fun NavGraphBuilder.aboutScreen(
    onNavigateUp: () -> Unit,
) {
    composable<AboutRoute> {
        AboutScreen(
            onNavigateUp = onNavigateUp,
        )
    }
}
