const url = 'http://10.0.0.208/update';
const bodyStart = {
    encender : 0,
    apagar : 0,
    desfase : 0,
    tempo: 0
}

const bodyConfiguracion = {
    tC : 0,
    te1 : 4,
    ta1 : 2,
    te2 : 5,
    ta2 : 13
}

const bodyArray = [bodyStart, bodyConfiguracion];

async function postRequest(url,data){
    const response = await fetch(url,{
        method: 'POST',
        headers:{
            'Content-Type' : 'application/json'
        },
        body:JSON.stringify(data)
    });
    return response;
}

postRequest(url,bodyStart)
.then(data => {
    console.log(data);
})