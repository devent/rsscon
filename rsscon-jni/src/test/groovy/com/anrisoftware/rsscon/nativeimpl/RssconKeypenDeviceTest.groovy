/*
 * Copyright 2011 Erwin Mueller <erwin.mueller@deventm.org>
 *
 * This file is part of rssconjava-native.
 *
 * rssconjava-native is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rssconjava-native is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rssconjava-native. If not, see <http://www.gnu.org/licenses/>.
 */
package com.anrisoftware.rsscon.nativeimpl

import groovy.util.logging.Slf4j

import org.junit.Test

import com.anrisoftware.globalpom.utils.TestUtils
import com.anrisoftware.rsscon.api.BaudRate
import com.anrisoftware.rsscon.api.RssconIOException
import com.anrisoftware.rsscon.utils.RssconTestUtils

/**
 * Test send start command and read data from the Keypen device.
 *
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
@Slf4j
class RssconKeypenDeviceTest extends RssconTestUtils {

	static final byte[] startCommand = [
		0x55,
		0xAA,
		0x80,
		0xFF,
		0xFF,
		0xFF
	]

	static final byte[] stopCommand = [
		0x55,
		0xAA,
		0x00,
		0xFF,
		0xFF,
		0xFF
	]

	@Test
	void "write start and read data no block no wait"() {
		if (!deviceAvailable) {
			return
		}

		def device = devicePath
		def baudRate = BaudRate.BAUDRATE_921600
		def rsscon = nativeFactory.create device, baudRate
		def outputStream = outputFactory.create rsscon
		def inputStream = inputFactory.create rsscon
		outputStream.write startCommand
		byte[] data = new byte[64 * 3]
		TestUtils.shouldFailWith(RssconIOException) { inputStream.read(data) }
		outputStream.write stopCommand
		outputStream.close()
	}

	@Test
	void "write start and read data block no wait"() {
		if (!deviceAvailable) {
			return
		}

		def device = devicePath
		def baudRate = BaudRate.BAUDRATE_921600
		def rsscon = nativeFactory.create device, baudRate
		rsscon.blocking = true
		def outputStream = outputFactory.create rsscon
		def inputStream = inputFactory.create rsscon
		outputStream.write startCommand
		byte[] data = new byte[64 * 3]
		(1..4).each {
			inputStream.read(data)
			log.info "Read {}", data
		}
		outputStream.write stopCommand
		outputStream.close()
	}


	@Test
	void "write start and read data block and wait"() {
		if (!deviceAvailable) {
			return
		}

		def device = devicePath
		def baudRate = BaudRate.BAUDRATE_921600
		def rsscon = nativeFactory.create device, baudRate
		rsscon.blocking = true
		rsscon.wait = true
		def outputStream = outputFactory.create rsscon
		def inputStream = inputFactory.create rsscon
		outputStream.write startCommand
		byte[] data = new byte[64 * 3]
		(1..4).each {
			inputStream.read(data)
			log.info "Read {}", data
		}
		outputStream.write stopCommand
		outputStream.close()
	}

	@Test
	void "write start and read data using a buffer"() {
		if (!deviceAvailable) {
			return
		}

		def device = devicePath
		def baudRate = BaudRate.BAUDRATE_921600
		def rsscon = nativeFactory.create device, baudRate
		rsscon.blocking = true
		def outputStream = outputFactory.create rsscon
		def inputStream = new BufferedInputStream(inputFactory.create(rsscon), 64 * 3 * 10)
		outputStream.write startCommand
		byte[] data = new byte[64 * 3]
		(1..4).each {
			inputStream.read(data)
			log.info "Read {}", data
		}
		outputStream.write stopCommand
		outputStream.close()
	}
}
