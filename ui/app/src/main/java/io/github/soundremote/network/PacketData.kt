package io.github.soundremote.network

import java.nio.ByteBuffer

interface PacketData {

    /**
     * Writes this packet data to the given ByteBuffer and increments its position by packet size.
     * @param dest [ByteBuffer] to write to.
     * @throws IllegalArgumentException if there are fewer than packet size bytes remaining in [dest].
     */
    fun write(dest: ByteBuffer)
}
