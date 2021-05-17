<template>
    <div>
        <div class="col-md-12" v-show="students.length>0">
            <h3>👨‍🎓 Students</h3>
            <transition-group name="fade">
                <div class="row mrb-10" :key="student.id" v-for="student in students">
                    <div class="input-group m-b-5">
                        <!-- <div class="input-group-prepend">
                            <div class="input-group-text">
                                <input
                                    type="checkbox"
                                    v-model="student.done"
                                    :checked="student.done"
                                    :value="student.done"
                                    v-on:change="updateTodo(student)"
                                    title="Mark as done?"
                                />
                            </div>
                        </div> -->
                        <div class="input-group-text">💳 Name : </div>
                        <input
                            type="text"
                            class="form-control"
                            :class="student.done?'todo__done':''"
                            v-model="student.name"
                            @keyup.enter="updateStudent(student)"
                            placeholder="Student name"
                        />
                        <div class="input-group-text">🕐 Age : </div>
                        <input type="text" class="form-control" v-model="student.age" @keyup.enter="updateStudent(student)" placeholder="Age">
                        <div class="input-group-text">🏫 University : </div>
                        <input type="text" class="form-control" v-model="student.university" @keyup.enter="updateStudent(student)" placeholder="University">
                        <div class="input-group-text">📚 Study program : </div>
                        <input type="text" class="form-control" v-model="student.study" @keyup.enter="updateStudent(student)" placeholder="Study Program">
                        
                        <div class="input-group-append">
                            <div class="input-group-text">
                                <button class="btn btn-danger"
                                    
                                    title="Delete todo?"
                                    v-on:click="deleteStudent(student.id)"
                                >🗑️ Delete</button>
                            </div>
                        </div>
                    </div>
                    <span class="help-block small" v-show="student.editing">Hit enter to update</span>
                </div>
            </transition-group>
        </div>

        <div class="row alert alert-info text-center" v-show="students.length==0">
            <p class="alert alert-warning">
                <strong>0 students</strong>
                <br /> 😢 You do not have any students in db. 😢
            </p>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
import bus from '../utils/bus';
import config from '../utils/config';
/* eslint-disable */
export default {
    data() {
        return {
            students: [],
        };
    },
    created: function() {
        this.fetchStudents();
        this.listenToEvents();
    },
    methods: {
        fetchStudents() {
            let uri = `${config.API_URL}/students`;
            axios.get(uri).then(response => {
                this.students = response.data;
            });
            console.log(this.students);
        },
        updateStudent(student) {
            let id = student.id;
            let uri = `${config.API_URL}/students/${id}`;
            student.editing = false;
            axios
                .put(uri, student)
                .then(response => {
                    console.log(response);
                })
                .catch(error => {
                    console.log(error);
                });
        },

        async deleteStudent(id) {
            let uri = `${config.API_URL}/students/${id}`;
            await axios.delete(uri);
            this.fetchStudents();
        },
        async deleteAllStudents() {
            let uri = `${config.API_URL}/students/`;
            await axios.delete(uri);
            this.fetchStudents();
        },

        listenToEvents() {
            bus.$on('refreshStudent', ($event) => {
                this.fetchStudents(); //update students
            });
        },
    },
};
/* eslint-enable */
</script>

<style scoped>
.todo__done {
    text-decoration: line-through !important;
}
.no_border_left_right {
    border-left: 0px;
    border-right: 0px;
}
.flat_form {
    border-radius: 0px;
}
.mrb-10 {
    margin-bottom: 10px;
}
.addon-left {
    background-color: none !important;
    border-left: 0px !important;
    cursor: pointer !important;
}
.addon-right {
    background-color: none !important;
    border-right: 0px !important;
}


.fade-enter-active, .fade-leave-active {
  transition: opacity .5s;
}
.fade-enter, .fade-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
</style>