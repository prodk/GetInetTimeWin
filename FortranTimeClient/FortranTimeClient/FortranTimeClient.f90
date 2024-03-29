!  (c) Mykola Prodanov, January 2013, Juelich, Germany.
!  FortranTimeClient.f90
!  Example of using functions from Timedll.dll library
!  to get internet time on Windows from Fortran using C library calls
!
!  Fortran Console Application 
!  Generated by PGI Visual Fortran(R)
!  1/27/2013 10:52:46 PM

!  Example
      program prog
      ! Export this subroutine for use by other executables      
      implicit none   
      
      ! Get year function
      interface
            integer function getyear
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'getyear' :: getyear     
            end function getyear
      end interface
      ! Get month function
      interface
            integer function getmonth
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'getmonth' :: getmonth     
            end function getmonth
      end interface
      ! Get day function
      interface
            integer function getday
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'getday' :: getday     
            end function getday
      end interface
      ! Get hour function
      interface
            integer function gethour
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'gethour' :: gethour     
            end function gethour
      end interface
      ! Get minute function
      interface
            integer function getminute
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'getminute' :: getminute     
            end function getminute
      end interface
      ! Get second function
      interface
            integer function getsecond
            !!DEC$ ATTRIBUTES STDCALL,DLLIMPORT,ALIAS: 'getsecond' :: getsecond     
            end function getsecond
      end interface 
      
      ! Variables     
      integer :: year
      integer :: month
      integer :: day
      integer :: hour
      integer :: minute
      integer :: second
      year = -1
      month = -1
      day = -1
      hour = -1
      minute = -1
      second = -1
      
      ! Body
      year = getyear()
      ! Check the output, function getyear() returns:
      ! returns -1 if socket problem
      if(year .eq. -1) then
        write(*,*) "Socket error"    
      end if    
      ! returns -2 if cannot connect to the server
      if(year .eq. -2) then
        write(*,*) "Cannot connect to server"
      end if
      ! returns -3 if connection is closed
      if(year .eq. -3) then
        write(*,*) "Connection to server is closed"
      end if
      ! returns -4 if recv failed
      if(year .eq. -4) then
        write(*,*) "Receiving data failed"
      end if
      ! returns -5 if parsing error
      if(year .eq. -5) then
        write(*,*) "Parsing error"
      end if
        
      ! get other data
      month = getmonth()
      day = getday()
      hour = gethour()
      minute = getminute()
      second = getsecond()    
      
      write(*,*) "year: ", year
      write(*,*) "month: ", month
      write(*,*) "day: ", day
      write(*,*) "hour: ", hour
      write(*,*) "minute: ", minute
      write(*,*) "second: ", second

      end program prog