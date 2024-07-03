"C:\Program Files\MRE SDK V3.0.00\tools\DllPackage.exe" "D:\MyGitHub\vsm_srv\vsm_srv.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy vsm_srv.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\vsm_srv.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

