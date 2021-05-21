function binary(n){
    let result = [];
    let bigger = 0;
    let secuential = 0;
    while(n>0){
        if(n%2 === 1){
            result.push(1);
        }else{
            result.push(0);
        }

        n = Math.floor((n/2));
    }  
    for(let i=result.length-1;i>=0;i--){
        if(result[i] === 1){
            secuential++;
        }else{
            if(bigger < secuential){
                bigger = secuential;
            }
            secuential = 0;
        }
            
    }
    if(bigger < secuential){
        bigger = secuential;
    }
    console.log(bigger);
}

binary(439);