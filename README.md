tmdblib
=======

TMDblib is a small API wrapper for accessing [The Movie DB](http://www.themoviedb.org/)'s JSON API from C++ projects.  
TMDblib is currently Windows-only, and uses wide strings for internal and external text representation.

Usage
-----

You'll need to compile TMDblib against [Boost](http://www.boost.org/) and the [POCO C++ libraries](http://pocoproject.org/).  
After that you'll only need the TMDb.h header and tmdblib.lib binary to use tmdblib in your project.  
TMDblib also uses [JSON Spirit](http://www.google.com/search?q=json-spirit) for JSON parsing, but a precompiled version is included in the repository.

Building
--------

Requires [Boost](http://www.boost.org/) and [POCO](http://pocoproject.org/) in global includes/libraries.  
Solution and project files are targeted for Visual Studio 2008.

Credits
-------

Boost C++ Libraries copyright © the Boost project.  
POCO C++ Libraries copyright © 2006-2012 Applied Informatics Software Engineering GmbH.  
JSON Spirit copyright © John W. Wilkinson and later developers.  
stdint.h copyright © 2006-2008 Alexander Chemeris.