# Comprobaciones para la entrega del proyecto parcial

## Demostracion funcional (ejecutar delante del profesor)
- Se muestra el menu y se inicia con Enter.
- La nave se mueve con WASD o flechas.
- Se puede disparar pulsando Espacio.
- Caen asteroides y hacen perder vidas al impactar.
- Enemigos disparan proyectiles y pueden ser destruidos.
- Aparece un jefe y, al derrotarlo, se muestra pantalla de victoria.
- Cuando vidas llega a cero aparece Game Over; R permite reiniciar.

## Fundamentos POO y dónde señalarlos
1. Asociación: `EnemigoNave::objetivo` -> `NaveJugador` (referencia no propietaria).
2. Agregación: `NaveJugador::armaEquipada` -> `Arma` (el nivel es propietario del arma).
3. Composición: `Juego::nivelActual` y `NivelEspacial::objetos` con `std::unique_ptr`.
4. Encapsulamiento: `Nave::vidas` es privado, con `getVidas` y `recibirDanio`.
5. Herencia: `NaveJugador` y `EnemigoNave` derivan de `Nave`; `JefeFinal` deriva de `EnemigoNave`.
6. Polimorfismo: `std::vector<std::unique_ptr<ObjetoJuego>>` llama a `actualizar` y `dibujar` de forma virtual.

## Git (prueba documental, no inventada)
- Mostrar la salida real de `git log --oneline --all --graph`.
- Mostrar `git status` y las ramas/colaboradores reales.
- Si usan GitHub, abrir los commits y contribuciones que realmente han hecho.

## Advertencias de alcance
- La demostración es un prototipo de figuras geométricas (sprites pendientes).
- El criterio de avance del 25% depende del alcance que el docente acepte.
- No afirmes que el codigo se compilo en el equipo hasta comprobarlo en Visual Studio.
