Char inpath[100]; char outpath[100]
#ifdef WIN32
	strncpy(inpath , "c:\\temp\\coursein\\p2-in.txt", sizeof(inpath)-1);
	strncpy(outpath, "c:\\fileio\\p2-out.txt", sizeof(outpath)-1);
#else
	strncpy(inpath, "~/temp/coursein/p2-in.txt", sizeof(inpath) - 1);
	strncpy(outpath, "/fileio/p2-out.txt", sizeof(outpath) - 1);
#endif
//now put the string terminators where they belong.

	inpath[sizeof(inpath) - 1] = '0';
	outpath[sizeof(outpath) - 1] = '0';
