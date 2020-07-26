# The MAXRF file management library

This library provides an API to perform the following operations on MAXRF data files:
    • Header management including creation, conversion, edition, and token extraction
    • Data format conversions
    • Multidetector hypercube summation
    • Writing operations
    • Lookup table computation
    • Data integrity and format checks
Additionally, it also provides an interface to handle and edit fields of the user configuration file pertaining to a particular instrument setup

The library is used by:
    • the MAXRF image renderer class to parse through the look up table and produce 8-bit indexed images
    • the DAQ file  writer to obtain a reference to a file descriptor with the correct header contents and format
    • the spectra visualization class to parse through spectral data files
    • and all other classes that need to access the configuration file

The API has two main classes:
    • the MAXRFDataFile which contains all methods pertaining to data conversions, header edition, format checks…
    • the MAXRFConfigurationFile which handles access to a single configuration file via means of semaphores and memory mapped files

Finally, this library serves as a submodule to a GUI wrapper that provides an graphical interface to perform all of the tasks outlined above


### Dependencies

The library uses the following libraries to assist in the parsing and edition of both configuration and data files:
    • the PugiXML library to  handle XML files creation

