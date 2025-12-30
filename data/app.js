(() => {
    const toggleLedBtn = document.getElementById('toggleLedBtn');

    toggleLedBtn.addEventListener('click', async () => {
        try {
            const response = await fetch('/led', {
                method: 'POST'
            });
            const result = await response.json();
            console.log('Server answer:', result);
        } catch (error) {
            console.error('Error:', error);
            alert('Error toggling LED');
        }
    });
})();