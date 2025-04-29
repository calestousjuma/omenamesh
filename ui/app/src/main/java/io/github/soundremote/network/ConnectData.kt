package io.github.soundremote.network

import io.github.soundremote.util.Net
import io.github.soundremote.util.Net.putUByte
import io.github.soundremote.util.Net.putUShort
import io.github.soundremote.util.PacketRequestIdType
import java.nio.ByteBuffer

data class ConnectData(@Net.Compression val compression: Int, val requestId: PacketRequestIdType) :
    PacketData {
    override fun write(dest: ByteBuffer) {
        require(dest.remaining() >= SIZE)
        dest.putUByte(Net.PROTOCOL_VERSION)
        dest.putUShort(requestId)
        dest.putUByte(compression.toUByte())
    }

    companion object {
        /**
         * unsigned 8bit    Protocol version
         *
         * unsigned 16bit   Request id
         *
         * unsigned 8bit    Compression
         */
        const val SIZE = 4
    }
}
