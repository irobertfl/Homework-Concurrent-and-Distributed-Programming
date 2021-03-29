const http = require('http')
const students = require('./data/students')
const {getStudents, getStudent, createStudent, updateStudent, removeStudent, removeAllStudents,createMultipleStudents} = require('./controllers/studentController')

const server = http.createServer((req,res) => {
    
    if(req.url === '/api/students' && req.method === 'GET'){
        //Get all the students
        getStudents(req,res)
    }
    else if(req.url.match(/\/api\/students\/([0-9]+)/) && req.method === 'GET'){
        const id = req.url.split('/')[3]
        console.log(id)
        getStudent(req,res,id)

    }
    else if(req.url === '/api/students' && req.method === 'POST'){
        createStudent(req,res)
    }
    else if(req.url === '/api/students/multi' && req.method === 'POST'){
        createMultipleStudents(req,res)
    }
    else if(req.url.match(/\/api\/students\/([0-9]+)/) && req.method === 'PUT'){
        const id = req.url.split('/')[3]
        updateStudent(req,res,id)
    }
    else if(req.url.match(/\/api\/students\/([0-9]+)/) && req.method === 'DELETE'){
        const id = req.url.split('/')[3]
        removeStudent(req,res,id)
    }
    else if(req.url === '/api/students' && req.method === 'DELETE'){
        removeAllStudents(req,res)
    }
    else{
        res.writeHead(404,{'Content-Type':'application/json'})
        res.end(JSON.stringify({message:'Route not found'}))
    }
})

const PORT = process.env.PORT || 5000

server.listen(PORT,() => console.log(`Server running on port ${PORT}`))