const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Módulo Rele</title>
    <link rel='stylesheet' href='./style.css'>
</head>
<body>
    <script>

        function OnClick()
        {
            var btOn = document.getElementById('btOn').disabled = true;
            var btOff = document.getElementById('btOff');
            if(btOff.disabled == true) btOff.disabled = false;
            
            const xhr = new XMLHttpRequest();
            xhr.open('GET', 'low');
            xhr.onload = function()
            {
                if(xhr.status === 200)
                {
                    if(btOff.disabled == true) btOff.disabled = false;
                }
                else
                {
                    console.log('Faield');
                }
            };
            xhr.send();
        }

        function OffClick()
        {
            var btOff = document.getElementById('btOff').disabled = true;
            var btOn = document.getElementById('btOn');
            if(btOn.disabled == true) btOn.disabled = false;
            
            const xhr = new XMLHttpRequest();
            xhr.open('GET', 'high');
            xhr.onload = function()
            {
                if(xhr.status === 200)
                {
                    if(btOn.disabled == true) btOn.disabled = false;
                }
                else
                {
                    console.log('Faield');
                }
            };
            xhr.send();
        }

    </script>
    <div class='center-screen'>
        <div class='center-div'>
            <h2>Pressione os botões</h2>
                <button id='btOn' class='buttonOn' onclick='OnClick()'>ON </button>
                <button id='btOff' class='buttonOff' onclick='OffClick()'>OFF</button>
        </div>
    </div>
</body>
</html>)rawliteral";

const char style_css[] PROGMEM = R"rawliteral(
html, body {
    height: 100%;
    background-color: rgb(36, 36, 36);
}


h2 {
    font-family: Verdana, Geneva, Tahoma, sans-serif;
    color: white;

}

button {
    border: none;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    transition-duration: 0.4s;
    cursor: pointer;
    font-size: 30px;
    border-radius: 5px;
    margin-right: 10px;
}

.disable {
    opacity: 0.6;
    cursor: not-allowed;
}

.center-screen {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    text-align: center;
    min-height: 100vh;
}

.center-div {
    margin: auto;
    padding: 10px;
    text-align: center;
    border-radius: 5px;
}


.buttonOn {
    background-color: #4CAF50;
    border: 2px chartreuse;
}

.buttonOn:hover {
    background-color: #218f00;
    color: rgb(0, 0, 0);
}

.buttonOff {
    background-color: #f44336;
    border: 2px red;
}

.buttonOff:hover {
    background-color: #ff0000;
    color: rgb(0, 0, 0);
}

button:disabled {
    opacity: 0.6;
    cursor: not-allowed;
}

button:disabled:hover {
    background-color: #5f5f5fab;
    border: 2px rgb(255, 255, 255);
})rawliteral";