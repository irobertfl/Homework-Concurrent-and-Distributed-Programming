<template>
    <div>
        <br>
        <form @submit.prevent>
            <div class="row">
            
            <div class="form-group">
                
                <div class="form-control">
                    <div >

                    </div>
                    <button class="btn btn-warning" v-if="!addOpened" @click="addOpened = !addOpened"> ➕ Add new student</button>
                    <button class="btn btn-danger" v-if="addOpened" @click="addOpened = !addOpened"> ⇦ Back</button>
                    <transition name="fade">
                        <div v-if="addOpened">
                            <h1>Create a Student</h1>
                            <label>💳 Student name</label>
                            <input
                                type="text"
                                class="form-control"
                                @keypress="typing=true"
                                placeholder="Student Name ?"
                                v-model="student"
                                @keyup.enter="addStudent($event)"
                            />
                            <br>
                            <label>🕐Student age</label>
                            <input
                                type="text"
                                class="form-control"
                                @keypress="typing=true"
                                placeholder="Student age ?"
                                v-model="age"
                                @keyup.enter="addStudent($event)"
                            />
                            <br>
                            <label>🏫 Student university</label>
                            <input
                                type="text"
                                class="form-control"
                                @keypress="typing=true"
                                placeholder="Student university ?"
                                v-model="university"
                                @keyup.enter="addStudent($event)"
                            />
                            <br>
                            <label>📚 Student study program</label>
                            <input
                                type="text"
                                class="form-control"
                                @keypress="typing=true"
                                placeholder="Student program ?"
                                v-model="study"
                                @keyup.enter="addStudent($event)"
                            />
                            <br>
                            <button class="btn btn-primary p-2 form-control" @click.prevent="addStudent($event)">Add Student</button>
                            <span class="help-block small text-center" v-show="typing">Hit enter to save</span>
                        </div>
                    </transition>
                </div>
            </div>
            <div class="form-group">
                
            </div>
            </div>
        </form>
    </div>
</template>

<script>
import axios from 'axios';
import bus from '../utils/bus';
import config from '../utils/config';

export default {
    data() {
        return {
            name:'',
            age: '',
            university:'',
            study:'',
            studyProgramsList : ['Batchelor','Master','PhD'],
            typing: false,
            addOpened:false
        };
    },
    methods: {
        addStudent(event) {
            if (event) event.preventDefault();
            let url = `${config.API_URL}/students`;
            let param = {
                name: this.student,
                age : this.age,
                university : this.university,
                study : this.study,
                done: false,
            };
            axios
                .post(url, param)
                .then(response => {
                    this.clearStudent();
                    this.refreshStudent();
                    this.typing = false;
                    console.log(response)
                })
                .catch(error => {
                    throw error;
                });
        },
        clearStudent() {
            this.student = '';
        },
        refreshStudent() {
            bus.$emit('refreshStudent');
        },
    },
};
</script>
<style>
.fade-enter-active, .fade-leave-active {
  transition: opacity .5s;
}
.fade-enter, .fade-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
</style>