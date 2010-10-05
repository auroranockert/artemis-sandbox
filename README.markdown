Artemis Sandbox
===============================================================================

The Artemis sandbox, a set of APIs pulled from the Artemis package manager, it
is used to sandbox fetching, extraction, building and installing packages so
none of the packages write outside of their domain.

It's an extended clone of the MacRuby APIs, credits for all the good parts of
the code to them, feel free to blame me for the bad parts.

Use
-------------------------------------------------------------------------------

It's not for protecting your program from intruders, it's for protecting your
program against itself, please respect that.

It is a work in progress, so if you want something stable and/or good, use Dia.

API
-------------------------------------------------------------------------------

Another thing, I might move the stuff in 'lib/artemis/sandbox' to a more
generic library, it doesn't really fit here better than in some sort of common
piece for Artemis.

Tests
-------------------------------------------------------------------------------

No, there are no tests.

Yes, there should be.

About
-------------------------------------------------------------------------------

Please send mail (jens@nockert.se) if you use it, I like 