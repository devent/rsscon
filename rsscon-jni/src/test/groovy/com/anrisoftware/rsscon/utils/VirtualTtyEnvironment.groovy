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
package com.anrisoftware.rsscon.utils

import org.apache.commons.exec.CommandLine
import org.apache.commons.exec.DefaultExecutor
import org.apache.commons.exec.ExecuteResultHandler
import org.apache.commons.exec.ExecuteWatchdog
import org.apache.commons.exec.PumpStreamHandler

/**
 * Starts the {@code socat} command to create a virtual serial port.
 *
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class VirtualTtyEnvironment {

	DefaultExecutor executor

	ByteArrayOutputStream output

	void startSocat() {
		output = new ByteArrayOutputStream()
		def cmdline = new CommandLine("socat")
		cmdline.addArgument "-d"
		cmdline.addArgument "-d"
		cmdline.addArgument "PTY PTY", false
		executor = new DefaultExecutor()
		executor.watchdog = new ExecuteWatchdog(60 * 1000)
		executor.streamHandler = new PumpStreamHandler(output)
		executor.execute cmdline, [
			onProcessComplete: { exitValue -> },
			onProcessFailed: { e ->
			}
		] as ExecuteResultHandler
		def outputString = output.toString()
		println outputString
		this
	}
}
