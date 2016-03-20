
#include <assert.h>
#include "File.h"

#define UNUSED(x) x


FileError File::open( FileHandle &fh, const char * const fileName, FileMode mode )
{
	
	DWORD dwDesiredAccess = getAccesType(mode);
	DWORD  lpSecurityAttributes = CREATE_ALWAYS;
	if(mode==FILE_READ||mode==0)
	{
	 
	  lpSecurityAttributes= OPEN_EXISTING;
	}
	else if(mode==FILE_WRITE)
	{
	 
	   lpSecurityAttributes= CREATE_NEW;
	}

	  FileError err=   FILE_SUCCESS;
	  mode;
	  fh = CreateFile(fileName,                // name of the write
                       dwDesiredAccess,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       lpSecurityAttributes ,     // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

	 if (fh == INVALID_HANDLE_VALUE) 
      { 
        err= FILE_OPEN_FAIL;
      }

	return err;
}

FileError File::close(const  FileHandle fh )
{
     FileError err =FILE_CLOSE_FAIL;
	 BOOL info;
	 DWORD lpdwFlags[100];
	 info= GetHandleInformation(fh,lpdwFlags);
    if(fh!=INVALID_HANDLE_VALUE && fh!=0 && info!=0)
	 {
	   if(CloseHandle(fh))
	   {
		 
 	    err=FILE_SUCCESS;
	   }
	}
	 return err;
}

FileError File::write( FileHandle fh, const void * const buffer, const size_t inSize )
{
	
	
	FileError err=   FILE_WRITE_FAIL;
    DWORD dwBytesWritten = 0;
	if(fh!=0 && fh!=INVALID_HANDLE_VALUE && buffer!=0)
	{
	  if(WriteFile(fh, buffer, inSize, &dwBytesWritten, NULL) )
	   {
		
	   err=FILE_SUCCESS;
	   }
	}
	return err;
}

FileError File::read( FileHandle fh,  void * const buffer, const size_t inSize )
{
	
	 FileError err=   FILE_READ_FAIL;
     DWORD dwBytesRead = 0;
	if(fh!=0 && fh!=INVALID_HANDLE_VALUE)
	{
	  if(ReadFile(fh,buffer,inSize,&dwBytesRead,NULL))
	 {
		err =  FILE_SUCCESS;
	 }
	}

	return err;
}

FileError File::seek( FileHandle fh, FileSeek seek, int offset )
{

	DWORD dwMoveMethod ;
	if(seek==FILE_SEEK_BEGIN)
		dwMoveMethod =FILE_BEGIN;
	else if(seek==FILE_SEEK_CURRENT)
	  dwMoveMethod= FILE_CURRENT;
	else
		dwMoveMethod=FILE_END;
	FileError err=   FILE_SUCCESS;
	DWORD dwPtr =SetFilePointer(fh,offset,NULL,dwMoveMethod);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
     { 
		 err=FILE_SEEK_FAIL;
 
     } 

	return err;
}

FileError File::tell( FileHandle fh, int &offset )
{	
	FileError err=   FILE_SUCCESS;

	offset  =SetFilePointer(fh,0,0,FILE_CURRENT);
	if(offset<0)
		err=FILE_TELL_FAIL;
	
	return err; 
}

FileError File::flush( FileHandle fh )
{
	
  FileError  err = FILE_FLUSH_FAIL;
  if(fh!=0 && fh!=INVALID_HANDLE_VALUE)
  {
	if(FlushFileBuffers(fh))
		err=FILE_SUCCESS;
  }
	return err;
}

DWORD File::getAccesType(const FileMode mode )
{
	DWORD dwDesiredAccess;
	if(mode==FILE_READ||mode==0)
	{
	  dwDesiredAccess =GENERIC_READ;
	
	}
	else if(mode==FILE_WRITE)
	{
	  dwDesiredAccess= GENERIC_WRITE;
	  
	}
	else 
	dwDesiredAccess= GENERIC_READ |GENERIC_WRITE;

	return dwDesiredAccess;

}