#ifndef __TTY_CALL__
#define __TTY_CALL__
/// tty session function - send a command line to tty and wait for data return.
/// once done, this function will trigger the callback
/// - parameter device: device to open
/// - parameter command: command to send
/// - parameter size: size of command
/// - parameter buffer: data buffer for receiving
/// - parameter szbuf: max size of buffer
/// - returns: size of data that actual received
extern int tty_session(const char * device,
                const unsigned char * command, const int szcmd,
                unsigned char * buffer, const int szbuf);

#endif
