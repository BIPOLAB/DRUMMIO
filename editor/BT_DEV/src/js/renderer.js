// renderer.js
import React from 'react';
import ReactDOM from 'react-dom';
import Pad from './pads'; // Importa el componente Pad


document.addEventListener('DOMContentLoaded', () => {
    const midiMonitorBody = document.getElementById('midiMonitorBody');

    // Acceder a los dispositivos MIDI
    navigator.requestMIDIAccess()
        .then((midiAccess) => {
            const inputs = midiAccess.inputs.values();

            for (let input = inputs.next(); input && !input.done; input = inputs.next()) {
                input.value.onmidimessage = (event) => {
                    // Reiniciar el color a rojo después de un tiempo (500 ms)
                    setTimeout(() => {
                        const ledElement = document.getElementById('led');
                        ledElement.style.backgroundColor = 'white';
                    }, 150);
                    // Limpiar la tabla y luego agregar la fila para el último mensaje
                    midiMonitorBody.innerHTML = ''; // Limpiar las filas existentes
                    updateMidiMonitor(event);
                };
            }
        })
        .catch((error) => {
            console.error('Error accessing MIDI devices:', error);
        });

    function updateMidiMonitor(event) {
        // ... (código existente)
    }

    // Renderiza el componente Pad en un contenedor
    const padContainer = document.getElementById('padContainer');
    ReactDOM.render(<Pad />, padContainer);
});
