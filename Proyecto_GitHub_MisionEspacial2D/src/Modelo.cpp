#include "Modelo.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

namespace {
    bool tocan(const ObjetoJuego& a, const ObjetoJuego& b) {
        return a.getLimites().findIntersection(b.getLimites()).has_value();
    }
}

Nave::Nave(sf::Vector2f pos, sf::Vector2f vel, int vida, sf::Color c, sf::Vector2f tam)
    : ObjetoJuego(pos, vel), vidas(vida), color(c), tamano(tam) {}

void Nave::actualizarInmunidad(float dt) { inmunidad = std::max(0.f, inmunidad - dt); }
void Nave::recibirDanio(int cantidad) {
    if (!activo || inmunidad > 0.f) return;
    vidas -= cantidad;
    inmunidad = 0.35f;
    if (vidas <= 0) desactivar();
}
void Nave::dibujar(sf::RenderWindow& v) const {
    sf::RectangleShape rect(tamano);
    rect.setPosition(posicion);
    rect.setFillColor(color);
    v.draw(rect);
}
sf::FloatRect Nave::getLimites() const { return sf::FloatRect(posicion, tamano); }

Proyectil::Proyectil(sf::Vector2f pos, sf::Vector2f vel, int dano, Bando origen)
    : ObjetoJuego(pos, vel), danio(dano), bando(origen) {}
void Proyectil::actualizar(float dt) {
    posicion += velocidad * dt;
    if (posicion.y < -25.f || posicion.y > ALTO + 25.f) desactivar();
}
void Proyectil::dibujar(sf::RenderWindow& v) const {
    sf::RectangleShape rect({7.f, 17.f});
    rect.setFillColor(bando == Bando::Jugador ? sf::Color::Yellow : sf::Color::Magenta);
    rect.setPosition(posicion);
    v.draw(rect);
}
sf::FloatRect Proyectil::getLimites() const { return sf::FloatRect(posicion, {7.f, 17.f}); }

Arma::Arma(int dano, float disparosPorSegundo)
    : danioBase(dano), cadencia(disparosPorSegundo) {}
std::unique_ptr<Proyectil> Arma::crearProyectil(sf::Vector2f origen, Bando bando) const {
    float rapidez = bando == Bando::Jugador ? -430.f : 255.f;
    return std::make_unique<Proyectil>(origen, sf::Vector2f{0.f, rapidez}, danioBase, bando);
}

NaveJugador::NaveJugador()
    : Nave({376.f, 510.f}, {260.f, 260.f}, 3, sf::Color(50, 215, 250), {48.f, 48.f}) {}
void NaveJugador::actualizar(float dt) {
    actualizarInmunidad(dt);
    esperaDisparo = std::max(0.f, esperaDisparo - dt);
    sf::Vector2f desplazamiento{0.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) desplazamiento.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) desplazamiento.x += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) desplazamiento.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) desplazamiento.y += 1.f;
    // Normalizacion diagonal para mantener una velocidad uniforme.
    if (desplazamiento.x != 0.f && desplazamiento.y != 0.f) desplazamiento *= 0.70710678f;
    posicion += sf::Vector2f(desplazamiento.x * velocidad.x * dt, desplazamiento.y * velocidad.y * dt);
    posicion.x = std::clamp(posicion.x, 0.f, ANCHO - tamano.x);
    posicion.y = std::clamp(posicion.y, 0.f, ALTO - tamano.y);
}
void NaveJugador::equiparArma(Arma* arma) { armaEquipada = arma; }
std::unique_ptr<Proyectil> NaveJugador::disparar() {
    if (!armaEquipada || esperaDisparo > 0.f || !activo) return nullptr;
    esperaDisparo = 1.f / armaEquipada->getCadencia();
    return armaEquipada->crearProyectil({posicion.x + tamano.x / 2.f - 3.f, posicion.y - 18.f}, Bando::Jugador);
}

Asteroide::Asteroide(float x, float rapidez, float tamano)
    : ObjetoJuego({x, -2.f * tamano}, {0.f, rapidez}), radio(tamano) {}
void Asteroide::actualizar(float dt) {
    posicion += velocidad * dt;
    if (posicion.y > ALTO + 30.f) desactivar();
}
void Asteroide::dibujar(sf::RenderWindow& v) const {
    sf::CircleShape circulo(radio);
    circulo.setPosition(posicion);
    circulo.setFillColor(sf::Color(150, 105, 60));
    circulo.setOutlineColor(sf::Color(215, 155, 85));
    circulo.setOutlineThickness(2.f);
    v.draw(circulo);
}
sf::FloatRect Asteroide::getLimites() const { return sf::FloatRect(posicion, {radio * 2.f, radio * 2.f}); }

EnemigoNave::EnemigoNave(sf::Vector2f pos, int vida, sf::Color c, sf::Vector2f tam)
    : Nave(pos, {0.f, 38.f}, vida, c, tam) {}
void EnemigoNave::actualizar(float dt) {
    actualizarInmunidad(dt);
    posicion += velocidad * dt;
    enfriamiento = std::max(0.f, enfriamiento - dt);
    if (posicion.y > ALTO + 70.f) desactivar();
}
std::unique_ptr<Proyectil> EnemigoNave::disparar() {
    if (!objetivo || !objetivo->estaActivo() || enfriamiento > 0.f || !estaActivo()) return nullptr;
    enfriamiento = 1.6f;
    return std::make_unique<Proyectil>(
        sf::Vector2f{posicion.x + tamano.x / 2.f, posicion.y + tamano.y},
        sf::Vector2f{0.f, 220.f}, 1, Bando::Enemigo);
}

JefeFinal::JefeFinal()
    : EnemigoNave({330.f, 45.f}, 9, sf::Color(200, 45, 165), {140.f, 85.f}) {
    velocidad = {0.f, 0.f};
}
void JefeFinal::actualizar(float dt) {
    actualizarInmunidad(dt);
    enfriamiento = std::max(0.f, enfriamiento - dt);
    posicion.x += std::sin(enfriamiento * 6.f + posicion.y) * 25.f * dt;
    posicion.x = std::clamp(posicion.x, 0.f, ANCHO - tamano.x);
    if (getVidas() <= 4) cambiarFase();
}
void JefeFinal::cambiarFase() { fase = 2; }
std::unique_ptr<Proyectil> JefeFinal::disparar() {
    if (!objetivo || !objetivo->estaActivo() || enfriamiento > 0.f || !estaActivo()) return nullptr;
    enfriamiento = fase == 1 ? 1.15f : 0.6f;
    return std::make_unique<Proyectil>(
        sf::Vector2f{posicion.x + tamano.x / 2.f, posicion.y + tamano.y},
        sf::Vector2f{0.f, fase == 1 ? 235.f : 315.f}, 1, Bando::Enemigo);
}

NivelEspacial::NivelEspacial() {
    armas.push_back(std::make_unique<Arma>(1, 5.f));
    auto nave = std::make_unique<NaveJugador>();
    jugador = nave.get();
    jugador->equiparArma(armas.front().get());
    agregarObjeto(std::move(nave));
}
void NivelEspacial::agregarObjeto(std::unique_ptr<ObjetoJuego> obj) {
    if (obj) objetos.push_back(std::move(obj));
}
float NivelEspacial::siguienteX() {
    // Generador determinista simple: evita depender de distribuciones externas.
    contadorAleatorio = contadorAleatorio * 1664525u + 1013904223u;
    return 15.f + static_cast<float>(contadorAleatorio % 710u);
}
void NivelEspacial::actualizar(float dt) {
    // Evitar que una pausa larga provoque saltos enormes.
    dt = std::min(dt, 0.05f);
    tiempo += dt;
    esperaAsteroide -= dt;
    esperaEnemigo -= dt;

    for (auto& obj : objetos) if (obj->estaActivo()) obj->actualizar(dt);

    // Añadir objetos DESPUES de iterar: no invalidamos el vector.
    std::vector<std::unique_ptr<ObjetoJuego>> nuevos;
    if (jugador && jugador->estaActivo() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        auto bala = jugador->disparar();
        if (bala) nuevos.push_back(std::move(bala));
    }
    for (auto& obj : objetos) {
        if (auto* enemigo = dynamic_cast<EnemigoNave*>(obj.get()); enemigo && enemigo->estaActivo()) {
            auto bala = enemigo->disparar();
            if (bala) nuevos.push_back(std::move(bala));
        }
    }
    if (esperaAsteroide <= 0.f && !jefeAparecio) {
        nuevos.push_back(std::make_unique<Asteroide>(siguienteX(), 160.f + tiempo * 2.f, 18.f + static_cast<float>(contadorAleatorio % 14u)));
        esperaAsteroide = 0.9f;
    }
    if (esperaEnemigo <= 0.f && tiempo > 4.f && !jefeAparecio) {
        auto enemigo = std::make_unique<EnemigoNave>(sf::Vector2f{siguienteX(), -45.f});
        enemigo->fijarObjetivo(jugador);
        nuevos.push_back(std::move(enemigo));
        esperaEnemigo = 4.5f;
    }
    if (tiempo >= 25.f && !jefeAparecio) {
        jefeAparecio = true;
        auto jefe = std::make_unique<JefeFinal>();
        jefe->fijarObjetivo(jugador);
        nuevos.push_back(std::move(jefe));
    }
    for (auto& obj : nuevos) agregarObjeto(std::move(obj));
    resolverColisiones();
    retirarInactivos();
    if (jugador) puntuacion = jugador->getPuntos();
}
void NivelEspacial::resolverColisiones() {
    if (!jugador || !jugador->estaActivo()) return;
    for (auto& obj : objetos) {
        if (!obj->estaActivo()) continue;
        if (auto* astro = dynamic_cast<Asteroide*>(obj.get())) {
            if (tocan(*jugador, *astro)) {
                jugador->recibirDanio(astro->getDanioContacto());
                astro->desactivar();
            }
        } else if (auto* enemigo = dynamic_cast<EnemigoNave*>(obj.get())) {
            if (tocan(*jugador, *enemigo)) jugador->recibirDanio(1);
        } else if (auto* bala = dynamic_cast<Proyectil*>(obj.get())) {
            if (bala->getBando() == Bando::Enemigo) {
                if (tocan(*jugador, *bala)) {
                    jugador->recibirDanio(bala->getDanio());
                    bala->desactivar();
                }
            } else {
                for (auto& blanco : objetos) {
                    if (!blanco->estaActivo() || blanco.get() == bala) continue;
                    if (auto* e = dynamic_cast<EnemigoNave*>(blanco.get())) {
                        if (tocan(*e, *bala)) {
                            e->recibirDanio(bala->getDanio());
                            bala->desactivar();
                            if (!e->estaActivo()) {
                                bool esJefe = dynamic_cast<JefeFinal*>(e) != nullptr;
                                jugador->agregarPuntos(esJefe ? 200 : 30);
                                if (esJefe) victoria = true;
                            }
                            break;
                        }
                    } else if (auto* a = dynamic_cast<Asteroide*>(blanco.get())) {
                        if (tocan(*a, *bala)) {
                            a->desactivar();
                            bala->desactivar();
                            jugador->agregarPuntos(5);
                            break;
                        }
                    }
                }
            }
        }
    }
}
void NivelEspacial::retirarInactivos() {
    objetos.erase(std::remove_if(objetos.begin(), objetos.end(),
        [this](const auto& ptr) {
            if (ptr.get() == jugador) return false; // conservar para consultar vidas y puntos
            return !ptr->estaActivo();
        }), objetos.end());
}
void NivelEspacial::dibujar(sf::RenderWindow& ventana) const {
    // Fondo con estrellas; no requiere recursos graficos externos.
    sf::RectangleShape fondo({static_cast<float>(ANCHO), static_cast<float>(ALTO)});
    fondo.setFillColor(sf::Color(7, 12, 35));
    ventana.draw(fondo);
    for (unsigned i = 0; i < 75; ++i) {
        float x = static_cast<float>((i * 131u + 17u) % ANCHO);
        float y = static_cast<float>((i * 97u + 33u) % ALTO);
        sf::CircleShape estrella(i % 5 == 0 ? 1.8f : 0.9f);
        estrella.setFillColor(sf::Color(175, 195, 235));
        estrella.setPosition({x, y});
        ventana.draw(estrella);
    }
    for (const auto& obj : objetos) if (obj->estaActivo()) obj->dibujar(ventana);
}
