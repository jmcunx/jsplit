## jsplit -- Split File

**Note:** Starting October 12 2023, Microsoft GitHub is forcing
me to use my 2FA access to my repositories via my Cell Phone
or some proprietary application.  So updates to this project
will no longer occur on GitHub.  Updates can be downloaded
from my gemini capsule:

* gemini://gem.sdf.org/jmccue/computer/repository.gmi
* gemini://gem.sdf.org/jmccue/computer/repository.gmi (mirror)

[See gemini wikipedia](https://en.wikipedia.org/wiki/Gemini_(protocol)#Software) for clients.

jsplit is a clone of split(1) but adds other options.

This was created for Systems that does not have split(1)

If split(1) is present, you should use that since it
is much much faster.

[j\_lib2](https://github.com/jmcunx/j_lib2) is an **optional** dependency.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
