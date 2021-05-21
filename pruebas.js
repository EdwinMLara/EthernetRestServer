var findPeakElement = function(nums) {
    let peak = 0;
    for (let i=1;i<nums.length;i++){
        if(nums[i-1] < nums[i])
            peak = i;
    }

    return peak;
};

/*nums = [1,2,3,1];
nums2 = [1,2,1,3,5,6,4];
console.log(findPeakElement(nums))*/

var findMedianSortedArrays = function(nums1, nums2) {
    let completeArray = [...nums1,...nums2];
    completeArray.sort(function(a,b){return a-b});
    let mod = completeArray.length % 2;
    let pos = Math.floor(completeArray.length/2);
    if(mod){
        return completeArray[pos]
    }else{
        return (completeArray[pos-1] + completeArray[pos])/2;
    }
};

/*nums1 = [1,3];
nums2 = [2];

nums3 = [1,2];
nums4 = [3,4];

console.log(findMedianSortedArrays(nums1,nums2))
console.log(findMedianSortedArrays(nums3,nums4))*/

function ListNode(val,next){
    this.val = (val === undefined ? 0 : val);
    this.next = (next === undefined ? null : next);
}

ListNode.prototype.insert = function (nodeList,data){
    if(nodeList.next === null){
        nodeList.next = new ListNode(data)
    }else{
        nodeList.insert(nodeList.next,data);
    }
}

ListNode.prototype.show = function(){
    let auxNodeList = this;
    while(auxNodeList !== null){
        console.log(auxNodeList.val);
        auxNodeList = auxNodeList.next;
    }
}

list = new ListNode(1);
list.insert(list,4);
list.insert(list,5);

list2 = new ListNode(1);
list2.insert(list2,3);
list2.insert(list2,4);

list3 = new ListNode(2);
list3.insert(list3,6);

let lists = [list,list2,list3];

var mergeKLists = function(lists) {
    if(lists.length === 0 || lists.length > Math.pow(10,4)){
        return null;
    }
    let auxArrayList = []
    lists.forEach(nodeList =>{
        if(nodeList){
            while(nodeList !== null){
                auxArrayList.push(nodeList.val);
                nodeList = nodeList.next;
            } 
        }  
    });

    if(auxArrayList.length > 0){
        auxArrayList.sort(function(a,b){return a-b});

        let sortedList = new ListNode(auxArrayList[0]);
        let auxlist = sortedList;
        for(let i = 1;i<auxArrayList.length;i++){
            auxlist.next = new ListNode(auxArrayList[i]);
            auxlist = auxlist.next;
        }
        return sortedList;
    }else{
        return null;
    }
    
};

/*let list4 = new ListNode(1);
let lists2 = [null,list4]
console.log(mergeKLists([null]));*/

var strFail = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ";
strFail2 = strFail.slice(0,30);

for(let i = 0; i<525 ; i++){
    strFail += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ";
}

strFail += strFail2;

var lengthOfLongestSubstring = function(s) {
    let maxlength = 5*Math.pow(10,4);
    let betterResult = 0;
    if(s.length >= 0 && s.length <= maxlength){
        
        let auxS = s;
        for(let i=0;i<s.length;i++){
            const auxSet = new Set();
           
            [...auxS].forEach(c =>{
                auxSet.add(c);
            });

            let srtResult = "";
            auxSet.forEach(item =>{
                return srtResult += item;
            });

            let auxSrtResult = srtResult;
            //check the right side
            while(s.indexOf(srtResult) === -1){
                srtResult = srtResult.slice(1,srtResult.length);
            }
            if(srtResult.length > betterResult){
                betterResult = srtResult.length;
            }

            //i need to check the left side
            while(s.indexOf(auxSrtResult) === -1){
                auxSrtResult = auxSrtResult.slice(0,(auxSrtResult.length-1));
            }
            if(auxSrtResult.length > betterResult){
                betterResult = auxSrtResult.length;
            }
            if(betterResult >= 95){
                break;
            }

            auxS = auxS.slice(1,auxS.length);
        }
        
    }

    return betterResult;
};

/*console.time("measureSubString")
console.log(lengthOfLongestSubstring(strFail));
console.timeEnd("measureSubString");

console.log("--------------------------------------");*/

var lengthOfLongestSubstring2 = function(s) {
    const auxSet2 = new Set(); 
    let betterResult = 1;
    let maxStrLength = 5*Math.pow(10,4);
    let auxS = s;

    if(s.length > 0 && s.length <= maxStrLength){
        for(let i = 0;i<s.length;i++){
            let auxi = 0;
            auxSet2.add(auxS[0]);
            for(let j=1;j<auxS.length;j++){
                if(auxSet2.has(auxS[j])){
                    let maxStrLength = (j-auxi);
                    auxi = j;
                    auxSet2.clear();
                    auxSet2.add(auxS[j]);
                    if(betterResult < maxStrLength){
                        betterResult = maxStrLength;
                    }
                }else{
                    auxSet2.add(auxS[j]);
                    if(j === (auxS.length -1)){
                        let maxStrLength = (j-auxi) + 1;
                        if(betterResult < maxStrLength){                 
                            betterResult = maxStrLength;
                        }
                    }
                }
            }
            auxSet2.clear();
            auxS = auxS.slice(1,auxS.length);
            if(betterResult >= 95){
                break;
            }
        }
        
        return betterResult;
    }else{
        return 0
    }   
}

/*let strDirecion = "pwwkew";
console.time("measureSubString2")
console.log(lengthOfLongestSubstring2(strFail));
console.timeEnd("measureSubString2");*/


const auxSumhourglass = [[-9, -9, -9, 1, 1, 1],
[0, -9, 0, 4, 3, 2],
[-9, -9, -9, 1, 2, 3],
[0, 0, 8, 6, 6, 0],
[0, 0, 0, -2, 0, 0],
[0, 0, 1, 2, 4, 0]];

function hourglassSum(arr) {
    let row = arr.length;
    let col = arr[0].length;
    let result = -1000;
    for (let i=1;i<row-1;i++){
        for (let k=1;k<col-1;k++){
            let sum = arr[i-1][k-1] + arr[i-1][k] + arr[i-1][k+1];
            sum += arr[i][k];
            sum += arr[i+1][k-1] + arr[i+1][k] + arr[i+1][k+1];
            if(sum>result){
                result = sum;
            }
        }
    }
    return result;
}

/*console.log("HackerRank");
console.log(auxSumhourglass);
console.log(hourglassSum(auxSumhourglass));*/

function solve(meal_cost, tip_percent, tax_percent) {
    let aux_tip = tip_percent/100;
    let aux_tax = tax_percent/100;
    let result  = meal_cost + (meal_cost*aux_tip) + (meal_cost*aux_tax);
    console.log(Math.round(result));
}


/*console.log("hacker rand day 3");
solve(12,20,8);*/

function processData(input){
    let straux = input.split('\n');
    let N = parseInt(straux[0]);

    for(let k=1;k<=N;k++){
        let par = "";
        let inpar = ""
        for(let i=0;i<straux[k].length;i++){
            if(i%2 != 0){
                inpar = inpar + straux[k][i];
            }else{
                par = par + straux[k][i];
            }
        }
        let result = par+" "+inpar;
        console.log(result);
    }
} 

/*let test = "2\nHacker\nRank";
console.log("hacker rand day 6");
processData(test);*/

const fs = require('fs');
const { resourceLimits } = require('node:worker_threads');
   
const data = fs.readFileSync('./input01.txt',
            {encoding:'utf8', flag:'r'});
function processData(input) {
    let difInput = input.split("\n");
    let test = difInput.length-difInput[0]-1;
    let can = parseInt(difInput[0]);
    let m = new Map();
    for (let i=1;i<=can;i++){
        let aux = difInput[i].split(" ");
        m.set(aux[0],aux[1]);
    }

    for(let i=1;i<=test;i++){
        if(m.has(difInput[can+i])){
            let strResul = `${difInput[can+i]}=${m.get(difInput[can+i])}`;
            console.log(strResul);
        }else{
            console.log("Not found");
        }
    }
}
/**Dia 7 creo de hacker rank 
processData(data);*/

function factorial(n){
    let fac = 1;
    if(n>0){
        fac = factorial(n-1) * n;
        return fac;
    }
    return fac;
}

/**Dia 9 creo de hacker rank 
factorial(3);*/


function binary(n){
    let result = [];
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
            break;
        }
    }
    console.log(result);
    console.log(secuential);
}

binary(439);