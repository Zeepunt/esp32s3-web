import axios from 'axios'

// 创建简单的 axios 实例，让浏览器处理认证
const apiClient = axios.create({
  baseURL: '',
  timeout: 10000,
})

export { apiClient }