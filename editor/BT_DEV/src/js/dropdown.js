const dropdownMenu = document.querySelector('.dropdown-menu');
const dropdownItems = Array.from(dropdownMenu.querySelectorAll('.dropdown-item'));
let selectedIndex = -1;

function updateSelection() {
    dropdownItems.forEach((item, index) => {
        if (index === selectedIndex) {
            item.classList.add('selected');
        } else {
            item.classList.remove('selected');
        }
    });
}

const prevButton = document.getElementById('prevButton');
const nextButton = document.getElementById('nextButton');

prevButton.addEventListener('click', () => {
    if (selectedIndex > 0) {
        selectedIndex--;
        updateSelection();
    }
});

nextButton.addEventListener('click', () => {
    if (selectedIndex < dropdownItems.length - 1) {
        selectedIndex++;
        updateSelection();
    }
});

dropdownMenu.addEventListener('click', (event) => {
    if (event.target.classList.contains('dropdown-item')) {
        selectedIndex = dropdownItems.indexOf(event.target);
        updateSelection();
        dropdownButton.innerHTML = event.target.innerHTML;
    }
});
