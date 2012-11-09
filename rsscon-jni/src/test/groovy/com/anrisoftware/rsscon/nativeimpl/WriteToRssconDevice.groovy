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

import org.junit.Test

import com.anrisoftware.rsscon.api.BaudRate
import com.anrisoftware.rsscon.utils.VirtualTtyEnvironment

/**
 * Test simple read operation with a virtual serial port.
 *
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class WriteToRssconDevice extends RssconTestUtils {

	@Test
	void "Write one byte"() {
		def device = '/dev/pts/4'
		def baudRate = BaudRate.BAUDRATE_921600
		def rsscon = nativeFactory.create device, baudRate
		def stream = outputFactory.create rsscon
		stream.write 'something'.getBytes()[0]
		stream.close()
	}
}
