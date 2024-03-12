const drumMode = [
    { id: 'kick-drum', keyCode: 81, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587165/fcc-drum-machine/kick-drums/kick-drum-7.mp3', keyTrigger: 'Q' },
    { id: 'Snare', keyCode: 87, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587362/fcc-drum-machine/snare-drums/snaredrum1.mp3', keyTrigger: 'W' },
    { id: 'Hi-Hat-Open', keyCode: 69, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587997/fcc-drum-machine/hi-hats/hihat1.mp3', keyTrigger: 'E' },
    { id: 'Hi-Hat-Close', keyCode: 65, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587997/fcc-drum-machine/hi-hats/hihat4.mp3', keyTrigger: 'A' },
    { id: 'Tom-Tom', keyCode: 83, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532588185/fcc-drum-machine/tom-toms/tomtomdrum3.mp3', keyTrigger: 'S' },
    { id: 'Crash', keyCode: 68, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587948/fcc-drum-machine/cymbals%202/cymbalcrash1.mp3', keyTrigger: 'D' },
    { id: 'Ride', keyCode: 90, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532582454/sample-swap/drums-and-single-hits/rides/RIDE_S_11.mp3', keyTrigger: 'Z' },
    { id: 'China', keyCode: 88, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532580120/sample-swap/drums-and-single-hits/china/big_china_cym.mp3', keyTrigger: 'X' },
    { id: 'Cowbell', keyCode: 67, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532587867/fcc-drum-machine/cowbells/cowbell1.mp3', keyTrigger: 'C' }
];

const synthMode = [
    { id: 'Horn', keyCode: 81, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674689/sample-swap/horn/horn-section-godown.mp3', keyTrigger: 'Q' },
    { id: 'Flute', keyCode: 87, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674456/sample-swap/flute/192_flute-madness.mp3', keyTrigger: 'W' },
    { id: 'Church-Organ', keyCode: 69, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674666/sample-swap/piano/almost-the-doors-organ.mp3', keyTrigger: 'E' },
    { id: 'Saxophone', keyCode: 65, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532675116/sample-swap/sax/sax-squeal-tore-my-brains-out.mp3', keyTrigger: 'A' },
    { id: 'Acoustic-Guitar', keyCode: 83, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674083/sample-swap/guitar/120_acoustic-guitar-picking1.mp3', keyTrigger: 'S' },
    { id: 'Piano', keyCode: 68, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674667/sample-swap/piano/096_salsa-piano-1.mp3', keyTrigger: 'D' },
    { id: 'Violin', keyCode: 90, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674483/sample-swap/violin/violin-loop-2.mp3', keyTrigger: 'Z' },
    { id: 'Choir', keyCode: 88, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674510/sample-swap/choir/Slavic-Choir_Eb_Aah.mp3', keyTrigger: 'X' },
    { id: 'Voice', keyCode: 67, src: 'https://res.cloudinary.com/dzsmdyknz/video/upload/v1532674533/sample-swap/voice/voice_woo.mp3', keyTrigger: 'C' }
];


class DrumPad extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            isActive: ''
        }

        this.handleClick = this.handleClick.bind(this);
        this.activatePad = this.activatePad.bind(this);
        this.handleKeyPress = this.handleKeyPress.bind(this);
        this.adjustVolume = this.adjustVolume.bind(this);
        this.deactivatedPad = this.deactivatedPad.bind(this);
    }

    componentDidMount() {
        document.addEventListener('keydown', this.handleKeyPress);
        document.addEventListener('load', this.adjustVolume);
    }

    componentWillUnmount() {
        document.removeEventListener('keydown', this.handleKeyPress);
    }


    handleKeyPress(e) {
        if (e.keyCode === this.props.keyCode || e.keyCode === this.props.keyTrigger.charCodeAt()) {
            this.handleClick();
            window.focus();
        }
    }

    handleClick() {
        const padsound = document.getElementById(this.props.keyTrigger);
        padsound.play();
        padsound.currentTime = 0;
        this.activatePad();
        this.props.handleDisplay(this.props.id.replace(/-/g, ' '));
        setTimeout(this.deactivatedPad, 200);
    }

    activatePad() {
        this.setState({
            isActive: 'light-up'
        });
    }

    deactivatedPad() {
        this.setState({
            isActive: ''
        });
    }

    adjustVolume(props) {
        this.audio.volume = this.props.handleVolume
    }

    render() {
        return (
            <div id={this.props.id} className={`drum-pad ${this.state.isActive}`} onClick={this.handleClick}>
                <p>{this.props.keyTrigger}</p>
                <audio ref={ref => this.audio = ref} className='clip' id={this.props.keyTrigger} src={this.props.src}>
                </audio>
            </div>
        );
    }
}

function DrumPadOff(props) {
    const offkeys = ['q', 'w', 'e', 'a', 's', 'd', 'z', 'x', 'c'];
    return offkeys.map((keys) => {
        return <div className="drum-pad"><p>{keys}</p></div>
    });
}

class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            power: true,
            display: 'Loading Presets...',
            soundMode: 'Drums',
            volumeVal: 0.4,
            drumPads: true
        }
        this.handlePowerClick = this.handlePowerClick.bind(this);
        this.handleDrumMode = this.handleDrumMode.bind(this);
        this.handleSynthMode = this.handleSynthMode.bind(this);
        this.handleVolume = this.handleVolume.bind(this);
        this.handleDisplay = this.handleDisplay.bind(this);
        this.switchSound = this.switchSound.bind(this);
        this.powerSound = this.powerSound.bind(this);
    }

    handlePowerClick() {
        this.setState({
            power: !this.state.power,
            display: this.state.display
        });
        this.powerSound();

    }

    handleDrumMode() {
        this.setState({
            soundMode: 'Drums',
            display: 'kick'
        });
        this.switchSound();
    }

    handleSynthMode() {
        this.setState({
            soundMode: 'Synth',
            display: 'horn'
        });
        this.switchSound();
    }

    handleVolume(e) {
        if (this.state.power) {
            this.setState({
                volumeVal: e.target.value
            })
        }
    }

    handleDisplay(display) {
        if (this.state.power) {
            this.setState({
                display
            })
        }
    }

    switchSound() {
        document.getElementById('btn-audio').play();
        document.getElementById('btn-audio').currentTime = 0;
    }

    powerSound() {
        document.getElementById('power-btn-audio').play();
        document.getElementById('power-btn-audio').currentTime = 0;
    }

    render() {
        const drumModeOn = drumMode.map(d => <DrumPad id={d.id} src={d.src} keyCode={this.keyCode} keyTrigger={d.keyTrigger} handleDisplay={this.handleDisplay} handleVolume={this.handleVolume} />);
        const synthModeOn = synthMode.map(s => <DrumPad id={s.id} src={s.src} keyCode={this.keyCode} keyTrigger={s.keyTrigger} handleDisplay={this.handleDisplay} handleVolume={this.handleVolume} />);
        const padOff = <DrumPadOff />;
        const activeSwitch = [<button id="bank_btn_drums" className={this.state.soundMode === 'Drums' ? 'bank-btn drums on' : 'bank-btn drums'} onClick={this.handleDrumMode}><span className="text">Drums</span></button>,
        <button id="bank_btn_synth" className={this.state.soundMode === 'Synth' ? 'bank-btn synth on' : 'bank-btn synth'} onClick={this.handleSynthMode}><span className="text">Synth</span></button>];
        const inactiveSwitch = [<button id="bank_btn_drums" className='bank-btn drums'><span className="text">Drums</span></button>,
        <button id="bank_btn_synth" className='bank-btn synth'><span className="text">Synth</span></button>];
        const clips = [].slice.call(document.getElementsByClassName('clip'));
        clips.forEach(sound => {
            sound.volume = this.state.volumeVal
        });
        return (
            <div id="drumwrap">
                <header id="branding">
                    <div className="brand-wrap">
                        <h1><span className="orange">Drum</span>Machine</h1> <p className="small">Powered by: React</p>
                    </div>
                    <div className="powerbtn-wrap">
                        <button id="power_btn" className={this.state.power ? 'powerbtn on' : 'powerbtn'} onClick={this.handlePowerClick}>
                            <i class="fas fa-power-off"></i>
                        </button>
                        <audio ref={ref => this.audio = ref} id="power-btn-audio" src="https://res.cloudinary.com/dzsmdyknz/video/upload/v1532750168/sample-swap/sfx-and-unusual-sounds/bleeps-blips-blonks-blarts-and-zaps/boip.mp3" class="btn-audio-efx">Your browser does not support the audio element.</audio>
                    </div>
                </header>
                <div id="drum-machine">
                    <div id="drum-pads">
                        {this.state.power ?
                            [this.state.soundMode === 'Drums' ? drumModeOn : synthModeOn] : padOff
                        }
                    </div>
                    <div id="display-section">
                        <div id="screen" className={this.state.power ? 'on' : ''}>
                            <div className="text-area">
                                <section className="section">
                                    <h2 id="display" className="text-code">{this.state.display}</h2>
                                </section>
                                <section className="section">
                                    <p className="text-code-small vol left">Volume: {Math.round(this.state.volumeVal * 100)}%</p>
                                    <p className="text-code-small mode right">Mode: {this.state.soundMode}</p>
                                </section>
                            </div>
                        </div>
                        <div id="volume">
                            <div className="volume-container">
                                <label className="volume-label"><i class="fas fa-volume-up"></i></label>
                                <input type="range" min="0" max="1" step="0.01" value={this.state.volumeVal} className="volume-slider" onChange={this.handleVolume} />
                            </div>
                        </div>
                        <div id="bank-switch">
                            {this.state.power ? activeSwitch : inactiveSwitch}
                            <audio ref={ref => this.audio = ref} id="btn-audio" src="https://res.cloudinary.com/dzsmdyknz/video/upload/v1532597915/sample-swap/switches/switch-it-on2.mp3" class="btn-audio-efx"></audio>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}

ReactDOM.render(<App />, document.getElementById('root'))