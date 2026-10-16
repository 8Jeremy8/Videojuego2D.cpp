#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

constexpr unsigned ANCHO = 800;
constexpr unsigned ALTO = 600;

enum class Bando { Jugador, Enemigo };

// Abstraccion y polimorfismo: todos los objetos se actualizan y dibujan igual.
class ObjetoJuego {
protected:
    sf::Vector2f posicion;
    sf::Vector2f velocidad;
    bool activo = true;
public:
    ObjetoJuego(sf::Vector2f p, sf::Vector2f v) : posicion(p), velocidad(v) {}
    virtual ~ObjetoJuego() = default;
    virtual void actualizar(float dt) = 0;
    virtual void dibujar(sf::RenderWindow& ventana) const = 0;
    virtual sf::FloatRect getLimites() const = 0;
    bool estaActivo() const { return activo; }
    void desactivar() { activo = false; }
    sf::Vector2f getPosicion() const { return posicion; }
};

class Nave : public ObjetoJuego {
private:
    int vidas;
    float inmunidad = 0.f;
protected:
    sf::Color color;
    sf::Vector2f tamano;
    void actualizarInmunidad(float dt);
public:
    Nave(sf::Vector2f pos, sf::Vector2f vel, int vida, sf::Color c, sf::Vector2f tam);
    virtual ~Nave() = default;
    int getVidas() const { return vidas; }
    void recibirDanio(int cantidad);
    void dibujar(sf::RenderWindow& ventana) const override;
    sf::FloatRect getLimites() const override;
};

class Proyectil : public ObjetoJuego {
    int danio;
    Bando bando;
public:
    Proyectil(sf::Vector2f pos, sf::Vector2f vel, int dano, Bando origen);
    void actualizar(float dt) override;
    void dibujar(sf::RenderWindow& ventana) const override;
    sf::FloatRect getLimites() const override;
    int getDanio() const { return danio; }
    Bando getBando() const { return bando; }
};

class Arma {
    int danioBase;
    float cadencia;
public:
    Arma(int dano, float disparosPorSegundo);
    std::unique_ptr<Proyectil> crearProyectil(sf::Vector2f origen, Bando bando) const;
    float getCadencia() const { return cadencia; }
};

class NaveJugador : public Nave {
    Arma* armaEquipada = nullptr; // Agregacion: NO es propietaria del arma.
    int puntos = 0;
    float esperaDisparo = 0.f;
public:
    NaveJugador();
    void actualizar(float dt) override;
    void equiparArma(Arma* arma);
    std::unique_ptr<Proyectil> disparar();
    void agregarPuntos(int cantidad) { puntos += cantidad; }
    int getPuntos() const { return puntos; }
};

class Asteroide : public ObjetoJuego {
    int danioContacto = 1;
    float radio;
public:
    Asteroide(float x, float rapidez, float tamano);
    void actualizar(float dt) override;
    void dibujar(sf::RenderWindow& ventana) const override;
    sf::FloatRect getLimites() const override;
    int getDanioContacto() const { return danioContacto; }
};

class EnemigoNave : public Nave {
protected:
    NaveJugador* objetivo = nullptr; // Asociacion: referencia no propietaria.
    float enfriamiento = 1.f;
public:
    EnemigoNave(sf::Vector2f pos, int vida = 2, sf::Color c = sf::Color(230, 90, 90), sf::Vector2f tam = {44.f, 38.f});
    void actualizar(float dt) override;
    void fijarObjetivo(NaveJugador* jugador) { objetivo = jugador; }
    virtual std::unique_ptr<Proyectil> disparar();
};

class JefeFinal : public EnemigoNave {
    int fase = 1;
public:
    JefeFinal();
    void actualizar(float dt) override;
    void cambiarFase();
    std::unique_ptr<Proyectil> disparar() override;
};

class NivelEspacial {
    std::vector<std::unique_ptr<ObjetoJuego>> objetos; // Composicion
    std::vector<std::unique_ptr<Arma>> armas;           // Composicion
    NaveJugador* jugador = nullptr;                    // Referencia a objeto del vector
    int puntuacion = 0;
    float tiempo = 0.f;
    float esperaAsteroide = 0.f;
    float esperaEnemigo = 0.f;
    bool jefeAparecio = false;
    bool victoria = false;
    unsigned contadorAleatorio = 17;
    float siguienteX();
    void resolverColisiones();
    void retirarInactivos();
public:
    NivelEspacial();
    void agregarObjeto(std::unique_ptr<ObjetoJuego> objeto);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana) const;
    NaveJugador* getJugador() const { return jugador; }
    int getPuntuacion() const { return puntuacion; }
    bool termino() const { return victoria || !jugador || !jugador->estaActivo(); }
    bool gano() const { return victoria; }
};
