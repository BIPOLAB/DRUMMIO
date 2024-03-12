document.addEventListener('DOMContentLoaded', () => {
    const ledElement = document.getElementById('led');
    const midiMonitorBody = document.getElementById('midiMonitorBody');


    let isOn = false; // Variable para rastrear si el LED está encendido o apagado

    function toggleLED() {
        isOn = !isOn;
        ledElement.style.backgroundColor = isOn ? 'green' : 'white';
    }

    // Acceder a los dispositivos MIDI
    navigator.requestMIDIAccess()
        .then((midiAccess) => {
            const inputs = midiAccess.inputs.values();

            for (let input = inputs.next(); input && !input.done; input = inputs.next()) {
                input.value.onmidimessage = (event) => {
                    // Cuando se recibe un mensaje MIDI, cambiar el color del LED a verde
                    ledElement.style.backgroundColor = 'green';

                    // Reiniciar el color a rojo después de un tiempo (500 ms)
                    setTimeout(() => {
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
        const messageType = event.data[0] & 0xF0; // Obtener los primeros 4 bits para determinar el tipo de mensaje

        let messageTypeText = '';
        if (messageType === 0x90) {
            messageTypeText = 'Note On';
        } else if (messageType === 0x80) {
            messageTypeText = 'Note Off';
        } else {
            // Otros tipos de mensaje MIDI
            messageTypeText = 'Otros';
        }

        const row = midiMonitorBody.insertRow(0);
        const channelCell = row.insertCell(0);
        const messageTypeCell = row.insertCell(1);
        const valueCell = row.insertCell(2);

        channelCell.innerHTML = event.data[0] & 0x0F; // Canal MIDI
        messageTypeCell.innerHTML = messageTypeText; // Tipo de mensaje
        valueCell.innerHTML = event.data[2]; // Valor
    }
});
