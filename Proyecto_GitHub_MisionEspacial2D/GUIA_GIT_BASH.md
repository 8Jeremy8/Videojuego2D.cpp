# Subir el proyecto a GitHub con Git Bash

## Opcion recomendada: ya creaste el repositorio en GitHub

1. Descomprime el ZIP en el Escritorio (quedara la carpeta `Proyecto_GitHub_MisionEspacial2D`).
2. Abre **Git Bash** en el Escritorio (clic derecho > Open Git Bash here).
3. Clona el repositorio que YA creaste (reemplaza URL):

```bash
git clone https://github.com/TU_USUARIO/NOMBRE_DEL_REPOSITORIO.git
```

4. Abre dos ventanas del Explorador: una para `Proyecto_GitHub_MisionEspacial2D` y otra para la carpeta del repositorio que acabas de clonar. Copia **el contenido** de la primera dentro de la segunda (no la carpeta entera). Si GitHub ya tenia README, revisa antes de sustituirlo. **No copies una carpeta `.git`**.
5. Abre Git Bash dentro de la carpeta clonada y ejecuta, de uno en uno:

```bash
git status
git add .
git commit -m "Primer avance del videojuego en C++ y SFML"
git push
```

6. Actualiza la pagina del repositorio y verifica que aparezcan `src`, `include`, `docs`, `assets`, `evidencias`, `CMakeLists.txt` y `README.md`.

## Si Git pide nombre y correo

```bash
git config --global user.name "TU NOMBRE"
git config --global user.email "TU_CORREO_DE_GITHUB"
```

Repite `git commit ...` cuando hayas configurado tu identidad.

## Importante para la rubrica

- No se inventa historial de trabajo; cada integrante debe hacer cambios y commits reales.
- `out/`, `.vs/`, `build/` y archivos `.exe` se excluyen gracias a `.gitignore`.
- En `docs/informe/` coloca tu informe real; no esta incluido en este ZIP.
- En `evidencias/` guarda tus capturas reales del avance y del trabajo en equipo.
