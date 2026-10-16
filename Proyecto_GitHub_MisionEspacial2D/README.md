# Mision Espacial 2D - Prototipo C++ / SFML 3

Prototipo de videojuego 2D alineado con el UML corregido de la evaluacion parcial de POO.

## Requisitos
- Windows + Visual Studio 2026 (componente 'Desarrollo de escritorio con C++').
- Git y CMake (incluidos o configurados en Visual Studio).
- Internet durante la primera configuracion: CMake descarga SFML 3.0.2.

## Abrir y ejecutar
1. Extrae el .zip en el Escritorio.
2. Visual Studio: Archivo > Abrir > Carpeta > `MisionEspacial2D_CppSFML`.
3. Espera a que finalice la configuracion CMake sin errores.
4. En el desplegable superior, selecciona `main.exe (bin/main.exe)`.
5. Ctrl + F5.

## Controles
- ENTER: empezar desde el menu.
- Flechas o WASD: mover nave.
- ESPACIO: disparar.
- R: reiniciar al terminar.
- M: volver al menu al terminar.

## Logica jugable
- La nave tiene 3 vidas.
- Asteroides descienden, causan dano si chocan.
- Enemigos aparecen despues de unos segundos y disparan.
- Los proyectiles tienen bando: jugador o enemigo, para evitar dano amigo.
- Tras unos 25 segundos se presenta un jefe. Derrotarlo termina la mision.
- Hay puntuacion y pantallas de menu, Game Over y victoria.
- Graficos provisionales con figuras geometricas; se podran reemplazar por sprites.

## Correspondencia con el UML corregido
- Abstraccion/herencia: `ObjetoJuego` <- `Nave`, `Asteroide`, `Proyectil`;
  `Nave` <- `NaveJugador`, `EnemigoNave`; `EnemigoNave` <- `JefeFinal`.
- Polimorfismo: `NivelEspacial` actualiza y dibuja `ObjetoJuego` sin conocer el tipo concreto.
- Encapsulamiento: `vidas` privado en `Nave`, `getVidas` y `recibirDanio`.
- Composicion: `Juego` es propietario de `NivelEspacial` via `unique_ptr`;
  `NivelEspacial` posee los objetos y armas con `unique_ptr`.
- Agregacion: `NaveJugador` solo guarda `Arma*`, cuya propiedad pertenece al nivel.
- Asociacion: `EnemigoNave` conserva `NaveJugador*` como objetivo sin poseerlo.
- Dependencia: `Arma::crearProyectil` fabrica un proyectil, cedido luego al nivel.
- Patron State: `EstadoMenu`, `EstadoPartida`, `EstadoGameOver` implementan `EstadoJuego`.

## Git: evidencia REAL, no ficticia
Este .zip incluye codigo y .gitignore, pero NO simula historial Git ni commits de otros integrantes.
Desde la carpeta del proyecto, en CMD o terminal integrada de VS:

    git init
    git add .
    git commit -m "Prototipo inicial de Mision Espacial 2D"

Posteriormente cada integrante debe trabajar en ramas o realizar cambios propios y registrar
commits con su identidad. Si ya usan un repositorio GitHub compartido, acuerden antes
que repo van a utilizar para evitar duplicar el historial.

## Limites de la entrega
- El codigo esta disenado para SFML 3 y el UML corregido.
- En este entorno de preparacion no hay SFML 3 ni acceso externo para descargarla;
  falta compilar/ejecutar en tu Visual Studio y corregir eventuales incompatibilidades.
- No se puede garantizar que represente exactamente el 25% sin acuerdo sobre alcance con el docente.
