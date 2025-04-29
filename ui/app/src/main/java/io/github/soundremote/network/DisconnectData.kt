package io.github.soundremote.network

import java.nio.ByteBuffer

class DisconnectData : PacketData {
    override fun write(dest: ByteBuffer) {}

    companion object {
        const val SIZE = 0
    }
}
