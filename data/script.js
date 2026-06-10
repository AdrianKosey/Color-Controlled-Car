const color = document.getElementById("color");
const accion = document.getElementById("accion");
const velocidad = document.getElementById("velocidad");
const obstaculos = document.getElementById("obstaculos");
const vueltasDerecha = document.getElementById("vueltasDerecha");
const vueltasIzquierda = document.getElementById("vueltasIzquierda");

const wsUri =  "ws://esp32.local/";
const webSocket = new WebSocket(wsUri);

webSocket.addEventListener("message", (e) => {
    const data = JSON.parse(e.data);
    color.innerHTML = data.color;
    accion.innerHTML = data.accion;
    velocidad.innerHTML = data.velocidad;
    obstaculos.innerHTML = data.obstaculos;
    vueltasDerecha.innerHTML = data.vueltasDerecha;
    vueltasIzquierda.innerHTML = data.vueltasIzquierda;
});