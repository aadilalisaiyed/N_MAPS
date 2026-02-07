import express from "express";
import cors from "cors";
import analyzeRoute from "./routes/analyze.js";

const app = express();

app.use(cors());
app.use(express.json());

// test route
app.get("/", (req, res) => {
  res.json({ status: "Node.js server running" });
});

// ML route
app.use("/api", analyzeRoute);

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Node server running on http://localhost:${PORT}`);
});
