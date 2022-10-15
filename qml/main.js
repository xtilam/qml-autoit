let autoIndex = 0
const listActions = {}

g.register = function (names, obj) {
    if(typeof names !== 'string') return
    if(!obj) return
    if(typeof obj !== 'object') return

    const listName = names.split('.')
    
    for (const key in obj) {
        ++autoIndex
        const action = obj[key].bind(obj)
        listActions[autoIndex] = action 
        au3.registerUI(listName, key, autoIndex)
    }
}

g.callUIMethod = function (id) {
	console.log(id)
    const data = au3.getUIData()
    const action = listActions[id]
    if(!action) return
    return action.apply(undefined, data)
}

g.sendSignal = function(signal, ...args){
    au3.sendSignal.call(au3, signal, args)
}
