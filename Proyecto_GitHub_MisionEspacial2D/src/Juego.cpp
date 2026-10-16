#include "Juego.hpp"
#include <optional>
#include <string>
#include <utility>

Juego::Juego()
    : ventana(sf::VideoMode({ANCHO, ALTO}), "Mision Espacial 2D - POO") {
    ventana.setFramerateLimit(60);
    // Fuente del sistema en Windows; si falta, el juego sigue funcionando.
    fuenteDisponible = fuente.openFromFile("C:/Windows/Fonts/arial.ttf");
}
void Juego::iniciar() { estadoActual = std::make_unique<EstadoMenu>(); }
void Juego::cambiarEstado(std::unique_ptr<EstadoJuego> nuevoEstado) {
    // Se aplica DESPUES de actualizar para no destruir un estado mientras ejecuta un metodo.
    estadoPendiente = std::move(nuevoEstado);
}
void Juego::crearNivel() { nivelActual = std::make_unique<NivelEspacial>(); }
void Juego::finalizarPartida(bool haGanado, int puntos) {
    victoria = haGanado;
    puntuacionFinal = puntos;
    cambiarEstado(std::make_unique<EstadoGameOver>());
}
void Juego::dibujarTexto(const std::string& texto, unsigned tamano, float x, float y, sf::Color color) {
    if (!fuenteDisponible) return;
    sf::Text t(fuente, texto, tamano);
    t.setPosition({x, y});
    t.setFillColor(color);
    ventana.draw(t);
}
void Juego::ejecutar() {
    sf::Clock reloj;
    while (ventana.isOpen()) {
        float dt = reloj.restart().asSeconds();
        while (const std::optional evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) ventana.close();
        }
        if (!ventana.isOpen()) break;
        if (estadoActual) estadoActual->actualizar(*this, dt);
        if (estadoPendiente) estadoActual = std::move(estadoPendiente);
        ventana.clear(sf::Color(9, 14, 31));
        if (estadoActual) estadoActual->dibujar(*this, ventana);
        ventana.display();
    }
}

void EstadoMenu::actualizar(Juego& juego, float) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        juego.crearNivel();
        juego.cambiarEstado(std::make_unique<EstadoPartida>());
    }
}
void EstadoMenu::dibujar(Juego& juego, sf::RenderWindow& ventana) {
    sf::RectangleShape decoracion({130.f, 85.f});
    decoracion.setPosition({335.f, 180.f});
    decoracion.setFillColor(sf::Color(30, 140, 210));
    ventana.draw(decoracion);
    juego.dibujarTexto("MISION ESPACIAL 2D", 42, 145.f, 90.f);
    juego.dibujarTexto("ENTER - Comenzar partida", 25, 220.f, 335.f);
    juego.dibujarTexto("Mueve con WASD o flechas | ESPACIO dispara", 19, 130.f, 395.f);
    juego.dibujarTexto("Sobrevive y derrota a la nave jefe", 19, 190.f, 435.f);
}

void EstadoPartida::actualizar(Juego& juego, float dt) {
    auto* nivel = juego.getNivel();
    if (!nivel) return;
    nivel->actualizar(dt);
    if (nivel->termino()) juego.finalizarPartida(nivel->gano(), nivel->getPuntuacion());
}
void EstadoPartida::dibujar(Juego& juego, sf::RenderWindow& ventana) {
    auto* nivel = juego.getNivel();
    if (!nivel) return;
    nivel->dibujar(ventana);
    int vidas = nivel->getJugador() ? nivel->getJugador()->getVidas() : 0;
    juego.dibujarTexto("VIDAS: " + std::to_string(vidas), 22, 14.f, 10.f);
    juego.dibujarTexto("PUNTOS: " + std::to_string(nivel->getPuntuacion()), 22, 600.f, 10.f);
    juego.dibujarTexto("WASD/FLECHAS: mover | ESPACIO: disparar", 14, 170.f, 568.f);
}

void EstadoGameOver::actualizar(Juego& juego, float) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        juego.crearNivel();
        juego.cambiarEstado(std::make_unique<EstadoPartida>());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
        juego.cambiarEstado(std::make_unique<EstadoMenu>());
    }
}
void EstadoGameOver::dibujar(Juego& juego, sf::RenderWindow& ventana) {
    sf::RectangleShape fondo({static_cast<float>(ANCHO), static_cast<float>(ALTO)});
    fondo.setFillColor(juego.gano() ? sf::Color(10, 65, 55) : sf::Color(105, 15, 25));
    ventana.draw(fondo);
    juego.dibujarTexto(juego.gano() ? "MISION COMPLETADA" : "GAME OVER", 56, juego.gano() ? 90.f : 205.f, 160.f);
    juego.dibujarTexto("Puntuacion: " + std::to_string(juego.getPuntuacionFinal()), 27, 260.f, 285.f);
    juego.dibujarTexto("R - Reiniciar partida", 25, 260.f, 355.f);
    juego.dibujarTexto("M - Volver al menu", 25, 260.f, 395.f);
}
