#pragma once
#include "Modelo.hpp"
#include <memory>
#include <string>

class Juego;

// Interfaz del patron State (realizacion: triangulo vacio + linea punteada).
class EstadoJuego {
public:
    virtual ~EstadoJuego() = default;
    virtual void actualizar(Juego& juego, float dt) = 0;
    virtual void dibujar(Juego& juego, sf::RenderWindow& ventana) = 0;
};

class EstadoMenu final : public EstadoJuego {
public:
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego, sf::RenderWindow& ventana) override;
};
class EstadoPartida final : public EstadoJuego {
public:
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego, sf::RenderWindow& ventana) override;
};
class EstadoGameOver final : public EstadoJuego {
public:
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego, sf::RenderWindow& ventana) override;
};

class Juego {
    sf::RenderWindow ventana;
    sf::Font fuente;
    bool fuenteDisponible = false;
    std::unique_ptr<EstadoJuego> estadoActual;
    std::unique_ptr<EstadoJuego> estadoPendiente;
    std::unique_ptr<NivelEspacial> nivelActual;
    int puntuacionFinal = 0;
    bool victoria = false;
public:
    Juego();
    void iniciar();
    void ejecutar();
    void cambiarEstado(std::unique_ptr<EstadoJuego> nuevoEstado);
    void crearNivel();
    NivelEspacial* getNivel() const { return nivelActual.get(); }
    int getPuntuacionFinal() const { return puntuacionFinal; }
    bool gano() const { return victoria; }
    void finalizarPartida(bool haGanado, int puntos);
    void dibujarTexto(const std::string& texto, unsigned tamano, float x, float y, sf::Color color = sf::Color::White);
};
