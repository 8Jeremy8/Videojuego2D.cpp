@echo off
cd /d "%~dp0"
echo === Preparando Git para este proyecto ===
where git >nul 2>nul
if errorlevel 1 (
  echo ERROR: Git no se encuentra en PATH.
  pause
  exit /b 1
)
if not exist ".git" (
  git init
  if errorlevel 1 goto error
)
git add .
if errorlevel 1 goto error
git commit -m "Prototipo inicial de Mision Espacial 2D"
if errorlevel 1 (
 echo.
 echo El commit no se completo. Puede que falte configurar user.name / user.email,
 echo o que los cambios ya estuvieran guardados en Git.
 echo Revisa el mensaje anterior.
)
git status
pause
exit /b 0
:error
echo Ocurrio un error. Revisa el mensaje anterior.
pause
exit /b 1
