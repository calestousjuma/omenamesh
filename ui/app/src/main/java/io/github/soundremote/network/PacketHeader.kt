package io.github.soundremote.network

import io.github.soundremote.util.Net
import io.github.soundremote.util.Net.putUByte
import io.github.soundremote.util.Net.putUShort
import io.github.soundremote.util.Net.uByte
import io.github.soundremote.util.Net.uShort
import io.github.soundremote.util.PacketCategoryType
import java.nio.ByteBuffer

data class PacketHeader(val category: PacketCategoryType, val packetSize: Int) {
    constructor(category: Net.PacketCategory, packetSize: Int) : this(category.value, packetSize)

    /**
     * Writes this header to the given ByteBuffer and increments its position by [SIZE].
     * @param dest [ByteBuffer] to write to.
     * @throws IllegalArgumentException if there are fewer than [SIZE] bytes remaining in [dest].
     */
    fun write(dest: ByteBuffer) {
        require(dest.remaining() >= SIZE)
        dest.putUShort(Net.PROTOCOL_SIGNATURE)
        dest.putUByte(category)
        dest.putUShort(packetSize.toUShort())
    }

    companion object {
        /*
        unsigned 16bit    protocol signature
        unsigned 8bit     packet category
        unsigned 16bit    packet size including header
        */
        /**
         *  Network packet header size in bytes
         */
        const val SIZE = 5

        /**
         * Read a [PacketHeader] from the source ByteBuffer which must contain exactly one datagram.
         * This method checks protocol signature and packet size. Increments [buffer] position by
         * [SIZE] on successful read, or by an arbitrary value on fail.
         * @param buffer [ByteBuffer] to read from
         * @return The header or null if the [buffer] doesn't contain a single datagram with a valid
         * header
         */
        fun read(buffer: ByteBuffer): PacketHeader? {
            if (buffer.remaining() < SIZE) return null
            val signature = buffer.uShort
            if (signature != Net.PROTOCOL_SIGNATURE) return null
            val category = buffer.uByte
            val packetSize = buffer.uShort.toInt()
            val header = PacketHeader(category, packetSize)
            return if (buffer.limit() != header.packetSize) null else header
        }
    }
}
