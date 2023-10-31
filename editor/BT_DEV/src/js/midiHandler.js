// Acceder a los dispositivos MIDI
navigator.requestMIDIAccess()
    .then((midiAccess) => {
        const inputs = midiAccess.inputs.values();

        for (let input = inputs.next(); input && !input.done; input = inputs.next()) {
            input.value.onmidimessage = (event) => {
                // Manejar el mensaje MIDI recibido
                console.log('MIDI message received:', event.data);
                // Puedes cambiar el color del LED aquí según el mensaje MIDI recibido
            };
        }
    })
    .catch((error) => {
        console.error('Error accessing MIDI devices:', error);
    });