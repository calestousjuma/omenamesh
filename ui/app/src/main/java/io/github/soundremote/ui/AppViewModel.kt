package io.github.soundremote.ui

import android.content.Context
import androidx.annotation.StringRes
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import io.github.soundremote.service.ServiceManager
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
internal class AppViewModel @Inject constructor(private val serviceManager: ServiceManager) :
    ViewModel() {

    private val _systemMessage = MutableStateFlow<Int?>(null)
    val systemMessage: StateFlow<Int?>
        get() = _systemMessage

    init {
        viewModelScope.launch {
            while (isActive) {
                val message = serviceManager.systemMessages.receive()
                setMessage(message.stringId)
            }
        }
    }

    fun bindConnection(context: Context) {
        serviceManager.bind(context)
    }

    fun unbindConnection(context: Context) {
        serviceManager.unbind(context)
    }

    private fun setMessage(@StringRes messageId: Int) {
        _systemMessage.value = messageId
    }

    fun messageShown() {
        _systemMessage.value = null
    }
}