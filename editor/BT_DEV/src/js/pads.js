// Pad.js
import React, { useState } from 'react';
import './pads.css';

const Pad = () => {
    const [isActive, setIsActive] = useState(false);

    const handleClick = () => {
        setIsActive(!isActive);
    }

    const padClass = isActive ? 'pad active' : 'drum-pad';

    return (
        <div className={padClass} onClick={handleClick}>
            Pad
        </div>
    );
}

export default Pad;
